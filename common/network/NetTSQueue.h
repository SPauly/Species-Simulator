/*
	MMO Client/Server Framework using ASIO
	"Happy Birthday Mrs Javidx9!" - javidx9
	Videos:
	Part #1: https://youtu.be/2hNdkYInj4g
	Part #2: https://youtu.be/UbjxGvrDrbw
	License (OLC-3)
	~~~~~~~~~~~~~~~
	Copyright 2018 - 2020 OneLoneCoder.com
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:
	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.
	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.
	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com
	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020
*/

#pragma once
#include "NetCommonInclude.h"

namespace sim
{
    namespace net
    {
        template <typename T>
        class TSQueue // Thread Save Queue -> basic Queue that blox it's scope during each operation to prevent multiple threads from reading and writing at the same time
        {
        public:
            TSQueue() = default;                  // default CTOR
            TSQueue(const TSQueue&) = delete;
            TSQueue(TSQueue<T> &&new_item)
            {
                std::scoped_lock lock(muxQueue);
                deqQueue = std::move(new_item.deqQueue);
            }       
            virtual ~TSQueue()
            {
            }

        public:
            TSQueue &operator=(TSQueue<T> &&new_item)
            {
                std::scoped_lock lock(muxQueue);
                deqQueue = std::move(new_item.deqQueue);
                return *this;
            }
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

                cv_push.notify_one();
            }

            void push_back(const T &item)
            {
                std::scoped_lock lock(muxQueue);
                deqQueue.emplace_back(std::move(item));

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
                std::unique_lock<std::mutex> wait_lock(muxQueue);

                while (deqQueue.empty())
                {
                    cv_push.wait(wait_lock);
                }
            }
        protected:
            std::mutex muxQueue;
            std::deque<T> deqQueue;
            std::condition_variable cv_push;

        };
    }
}