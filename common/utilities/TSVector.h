#pragma once
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>

namespace sim
{
    //HelperType provides T with a shared_mutex to lock the element while writer accesses it
    template <typename T>
    struct HelperType
    {
        mutable std::shared_mutex mux{};
        T obj;

        HelperType() = default;
        
        HelperType(const T &_obj) : obj(_obj)
        {
        }

        HelperType(const HelperType<T> &_obj)
        {
            std::unique_lock<std::shared_mutex> lock(mux);
            obj = _obj.obj;
        }

        HelperType<T> &operator=(const T &new_item)
        {
            std::unique_lock<std::shared_mutex> lock(mux);
            obj = new_item;
            return *this;
        }

        HelperType<T> &operator=(const HelperType<T> &new_item)
        {
            std::unique_lock<std::shared_mutex> lock(mux);
            obj = new_item.obj;
            return *this;
        }
    };

    template <typename T>
    class TSVector
    {
    protected:
        bool BLOCKED = false;
        mutable std::shared_mutex muxVec;
        std::vector<HelperType<T>> vec;
        std::shared_ptr<std::condition_variable_any> cv_ptr = nullptr;

    public:
        TSVector()
        {
            cv_ptr = std::make_shared<std::condition_variable_any>();
        }
        TSVector(std::shared_ptr<std::condition_variable_any> cv)
            : cv_ptr(cv)
        {
            if (!cv_ptr)
                cv_ptr = std::make_shared<std::condition_variable_any>();
        }
        TSVector(const TSVector<T> &new_vec)
        {
            std::unique_lock<std::shared_mutex> lock(muxVec);
            vec = new_vec.vec;
            cv_ptr = new_vec.cv_ptr;
        };

        TSVector(TSVector<T> &&new_vec)
        {
            std::unique_lock<std::shared_mutex> lock(muxVec);
            vec = std::move(new_vec.vec);
            cv_ptr = std::move(new_vec.cv_ptr);
        }

        virtual ~TSVector()
        {
            if (cv_ptr)
                cv_ptr.reset();
            clear();
        }

    public:
        TSVector<T> &operator=(const TSVector<T> &&new_vec)
        {
            std::unique_lock<std::shared_mutex> lock(muxVec);

            vec = std::move(new_vec.vec);
            cv_ptr = std::move(new_vec.cv_ptr);

            cv_ptr->notify_one();

            return *this;
        }

        TSVector<T> &operator=(const TSVector<T> &new_vec)
        {
            std::unique_lock<std::shared_mutex> lock(muxVec);

            vec = new_vec.vec;
            cv_ptr = new_vec.cv_ptr;

            cv_ptr->notify_one();

            return *this;
        }

        //!!! use at_mutable() instead if T has to be thread save
        T &at(size_t pos)
        {
            std::shared_lock<std::shared_mutex> lock(muxVec);
            return vec.at(pos).obj;
        }

        HelperType<T> &at_mutable(size_t pos)
        {
            std::unique_lock<std::shared_mutex> lock(muxVec);
            cv_ptr->notify_one();
            return vec.at(pos);
        }

        const T &front()
        {
            std::shared_lock<std::shared_mutex> lock(muxVec);
            return vec.front().obj;
        }

        const T &back()
        {
            std::shared_lock<std::shared_mutex> lock(muxVec);
            return vec.back().obj;
        }

        void push_back(const T &item)
        {
            std::unique_lock<std::shared_mutex> lock(muxVec);
            vec.push_back(HelperType<T>{std::move(item)});
            cv_ptr->notify_one();
            return;
        }

        bool empty()
        {
            std::shared_lock<std::shared_mutex> lock(muxVec);
            return vec.empty();
        }

        size_t size()
        {
            std::shared_lock<std::shared_mutex> lock(muxVec);
            return vec.size();
        }

        void clear()
        {
            std::unique_lock<std::shared_mutex> lock(muxVec);
            vec.clear();
        }

        T pop_back()
        {
            std::unique_lock<std::shared_mutex> lock(muxVec);
            auto t = std::move(vec.back());
            vec.pop_back();
            return t;
        }

        void resize(size_t size)
        {
            std::unique_lock<std::shared_mutex> lock(muxVec);
            vec.resize(size);
            return;
        }

        void resize(size_t size, const T &init_var)
        {
            std::unique_lock<std::shared_mutex> lock(muxVec);
            vec.resize(size, init_var);
            return;
        }

        void wait(std::shared_ptr<std::condition_variable_any> custom_ptr = nullptr)
        {
            std::unique_lock<std::shared_mutex> wait_lock(muxVec);

            // change cv_ptr to custom ptr if necessary
            bool CHANGED_CVPTR = false;
            std::shared_ptr<std::condition_variable_any> prev_ptr = cv_ptr;

            if (custom_ptr)
            {
                cv_ptr = custom_ptr;
                CHANGED_CVPTR = true;
            }

            // actual wait -> may want to add a loop with appropriate condition here
            cv_ptr->wait(wait_lock);

            // change ptr back if it was changed
            if (CHANGED_CVPTR)
                cv_ptr = prev_ptr;
        }

        std::vector<T> get_vector()
        {
            std::vector<T> ret_vec;
            
            std::unique_lock<std::shared_mutex> lock(muxVec);

            for(int i = 0; i < vec.size(); i++)
            {
                ret_vec.push_back(vec.at(i).obj);
            }

            return ret_vec;
        }
    };
}