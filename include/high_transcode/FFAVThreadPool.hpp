#pragma once

#include <iostream>
#include <thread>
#include <future>
#include <queue>
#include <type_traits>
#include <functional>
#include <vector>
#include <memory>

namespace ff {
    template <typename R>
    class UThreadPool {
    public:
        explicit FFAVThreadPool(unsigned int threadCount = std::thread::hardware_concurrency())
        : isRunning(false) {
            this->createThread(threadCount);
        }

        virtual ~FFAVThreadPool() {
            this->stop();
        }

    public:
        template <typename Func, typename... Args>
        std::future<R> enqueue(Func&& func, Args&&... args) {
            auto task = std::make_shared<std::packaged_task<R()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
            std::future<R> result = task->get_future();
            {
                std::unique_lock<std::mutex> lockGuard(this->mutex);
                this->taskQueue.emplace([task]() {
                    (*task)();
                });
            }
            this->condition.notify_one();
            return result;
        }

        size_t getThreadCount() {
            return this->threadList.size();
        }

        void stop() {
            {
                std::unique_lock<std::mutex> lockGuard(this->mutex);
                this->isRunning = false;
            }
            this->condition.notify_all();

            for (auto& thread : this->threadList) {
                if (thread.joinable() == false) {
                    continue;
                }
                thread.join();
            }
        }

        size_t getTaskQueueSize() {
            return this->taskQueue.size();
        }

    private:
        void createThread(unsigned int threadCount) {
            for (unsigned int i = 0; i < threadCount; i++) {
                this->threadList.emplace_back([this](){
                    while (true) {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lockGaurd(this->mutex);
                            this->condition.wait(lockGaurd, [this](){ return this->isRunning == false|| !this->taskQueue.empty();});
                            if (this->isRunning         == false &&
                                this->taskQueue.empty() == true) {
                                return;
                            }

                            task = std::move(this->taskQueue.front());
                            this->taskQueue.pop();
                        }
                        task();
                    }
                });

                this->isRunning = true;
            }
        }

    private:
        bool isRunning;

        std::vector<std::thread> threadList;

        std::mutex mutex;
        std::condition_variable condition;

        std::queue<std::function<void()>> taskQueue;
    };
}