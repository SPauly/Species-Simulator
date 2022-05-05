#pragma once
#include <vector>
#include <thread>
#include <mutex>

namespace sim
{
    template <typename T>
    class TSVector
    {
    protected:
        std::mutex muxQueue;
        std::vector<T> vec;
        std::condition_variable *cv_ptr = nullptr;

    public:
        TSVector(std::condition_variable &cv = nullptr)
            : cv_ptr(cv)
        {
            if (!cv_ptr)
                cv_ptr = new std::condition_variable;
        }
        TSVector(const TSVector<T> &) = delete;

        virtual ~TSVector()
        {
            if (cv_ptr)
                delete cv_ptr;
            clear();
        }

    public:
        TSVector<T> &operator=(const std::vector<T> &new_vec)
        {
            std::scoped_lock lock(muxQueue);

            if (this->vec == &new_vec)
                return this;

            std::copy(new_vec.begin(), new_vec.end(), std::back_inserter(vec));
            cv_ptr->notify_one();
            return this;
        }

        T &at(size_t pos)
        {
            std::scoped_lock lock(muxQueue);
            return vec.at(pos);
        }

        std::vector<T> *const raw() // warning! this function enables non threadsave access to vector
        {
            return &vec;
        }

        const T &front()
        {
            std::scoped_lock lock(muxQueue);
            return vec.front();
        }

        const T &back()
        {
            std::scoped_lock lock(muxQueue);
            return vec.back();
        }

        T *data()
        {
            std::scoped_lock lock(muxQueue);
            return vec.data();
        }

        void push_back(const T &item)
        {
            std::scoped_lock lock(muxQueue);
            vec.emplace_back(std::move(item));
            cv_ptr->notify_one();
            return;
        }

        bool empty()
        {
            std::scoped_lock lock(muxQueue);
            return vec.empty();
        }

        size_t size()
        {
            std::scoped_lock lock(muxQueue);
            return vec.size();
        }

        void clear()
        {
            std::scoped_lock lock(muxQueue);
            vec.clear();
        }

        T pop_back()
        {
            std::scoped_lock lock(muxQueue);
            auto t = std::move(vec.back());
            vec.pop_back();
            return t;
        }

        void resize(size_t size)
        {
            std::scoped_lock lock(muxQueue);
            vec.resize(size);
            return;
        }

        void resize(size_t size, const T &initial)
        {
            std::scoped_lock lock(muxQueue);
            vec.resize(size, initial);
            return;
        }

        void wait()
        {
            std::unique_lock<std::mutex> wait_lock(muxQueue);

            while (vec.empty())
            {
                cv_ptr->wait(wait_lock);
            }
        }
    };
}