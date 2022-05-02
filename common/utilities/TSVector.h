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
        std::condition_variable cv_push;

    public:
        TSVector() = default;
        TSVector(const TSVector<T> &) = delete; // delete copy constructor so no reading when copying
        virtual ~TSVector()
        {
            clear();
        }

    public:
        std::vector<T> * const raw() // warning! this function is not enables non threadsave access to vector
        {
            return &vec;
        }
        // implement all functions needed to operate the vector together with locking the scope
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

        void push_front(const T &item)
        {
            std::scoped_lock lock(muxQueue);
            vec.emplace_front(std::move(item));

            cv_push.notify_one();
        }

        void push_back(const T &item)
        {
            std::scoped_lock lock(muxQueue);
            vec.emplace_back(std::move(item));

            cv_push.notify_one();
        }

        bool empty()
        {
            std::scoped_lock lock(muxQueue);
            return vec.empty();
        }

        size_t count()
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
            auto t = std::move vec.back();
            vec.pop_back();
            return t;
        }

        void wait()
        {
            std::unique_lock<std::mutex> wait_lock(muxQueue);

            while (vec.empty())
            {
                cv_push.wait(wait_lock);
            }
        }
    };
}