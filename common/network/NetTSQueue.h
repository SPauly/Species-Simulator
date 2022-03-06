#pragma once
#include "NetCommonInclude.h"

namespace sim
{
    namespace net
    {
        template <typename T>
        class TSQueue // Thread Save Queue -> basic Queue that blox it's scope during each operation to prevent multiple threads from reading and writing at the same time
        {
        protected:
            std::mutex muxQueue;
            std::deque<T> deqQueue;
            std::condition_variable cv_push;
            std::mutex muxWait;

        public:
            TSQueue() = default;                  // default CTOR
            TSQueue(const TSQueue<T> &) = delete; // delete copy constructor
            virtual ~TSQueue()
            {
                clear();
            }

        public:
            // implement all functions needed to operate the queue together with locking the scope
            const T &front()
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.front();
            }

            const T &back()
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.back();
            }

            void push_front(const T &item)
            {
                std::scoped_lock lock(muxQueue);
                deqQueue.emplace_front(std::move(item));

                std::unique_lock<std::mutex> wait_lock(muxWait);
                cv_push.notify_one();
            }

            void push_back(const T &item)
            {
                std::scoped_lock lock(muxQueue);
                deqQueue.emplace_back(std::move(item));

                std::unique_lock<std::mutex> wait_lock(muxWait);
                cv_push.notify_one();
            }

            bool empty()
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.empty();
            }

            size_t count()
            {
                std::scoped_lock lock(muxQueue);
                return deqQueue.size();
            }

            void clear()
            {
                std::scoped_lock lock(muxQueue);
                deqQueue.clear();
            }

            T pop_front()
            {
                std::scoped_lock lock(muxQueue);
                auto t = std::move(deqQueue.front());
                deqQueue.pop_front();
                return t;
            }

            T pop_back()
            {
                std::scoped_lock lock(muxQueue);
                auto t = std::move(deqQueue.back);
                deqQueue.pop_back();
                return t;
            }

            void wait()
            {
                while (empty())
                {
                    std::unique_lock<std::mutex> wait_lock(muxWait);
                    cv_push.wait(wait_lock);
                }
            }
        };
    }
}