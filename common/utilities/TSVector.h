#pragma once
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace sim
{
    template <typename T>
    class TSVector
    {
    protected:
        std::mutex muxVec;
        std::vector<T> vec;
        std::shared_ptr<std::condition_variable> cv_ptr = nullptr;

    public:
        TSVector()
        {
            cv_ptr = std::make_shared<std::condition_variable>();
        }
        TSVector(std::shared_ptr<std::condition_variable> cv)
            : cv_ptr(cv)
        {
            if (!cv_ptr)
                cv_ptr = std::make_shared<std::condition_variable>();
        }
        TSVector(const TSVector<T> &) = delete;

        virtual ~TSVector()
        {
            if (cv_ptr)
                cv_ptr.reset();
            clear();
        }

    public:
        TSVector<T> &operator=(const std::vector<T> &new_vec)
        {
            std::scoped_lock lock(muxVec);

            if (this->vec == &new_vec)
                return this;

            std::copy(new_vec.begin(), new_vec.end(), std::back_inserter(vec));
            cv_ptr->notify_one();
            return this;
        }

        T &at(size_t pos)
        {
            std::scoped_lock lock(muxVec);
            return vec.at(pos);
        }

        std::vector<T> *const raw() // warning! this function enables non threadsave access to vector
        {
            return &vec;
        }

        const T &front()
        {
            std::scoped_lock lock(muxVec);
            return vec.front();
        }

        const T &back()
        {
            std::scoped_lock lock(muxVec);
            return vec.back();
        }

        T *data()
        {
            std::scoped_lock lock(muxVec);
            return vec.data();
        }

        void push_back(const T &item)
        {
            std::scoped_lock lock(muxVec);
            vec.emplace_back(std::move(item));
            cv_ptr->notify_one();
            return;
        }

        bool empty()
        {
            std::scoped_lock lock(muxVec);
            return vec.empty();
        }

        size_t size()
        {
            std::scoped_lock lock(muxVec);
            return vec.size();
        }

        void clear()
        {
            std::scoped_lock lock(muxVec);
            vec.clear();
        }

        T pop_back()
        {
            std::scoped_lock lock(muxVec);
            auto t = std::move(vec.back());
            vec.pop_back();
            return t;
        }

        void resize(size_t size)
        {
            std::scoped_lock lock(muxVec);
            vec.resize(size);
            return;
        }

        void resize(size_t size, const T &init_var)
        {
            std::scoped_lock lock(muxVec);
            vec.resize(size, init_var);
            return;
        }

        void wait()
        {
            std::unique_lock<std::mutex> wait_lock(muxVec);

            while (vec.empty())
            {
                cv_ptr->wait(wait_lock);
            }
        }
    };
}