#include <condition_variable>
#include <mutex>
#include <queue>

namespace ff {
    template <typename T>
    class FFAVThreadSafeQueue {
    public:
        FFAVThreadSafeQueue(size_t maxSize) : maxSize(maxSize), close(false) {}

        void push(T& item) {
            std::unique_lock<std::mutex> lock(this->pushConditionMutex);
            pushCondition.wait(lock, [this] {
                if (this->isClosed() == true || queue.size() < maxSize) {
                    return true;
                }
                return false;
            });

            if (this->isClosed() == true) {
                return;
            }

            {
                std::lock_guard<std::mutex> lockGuard(this->queueMutex);
                queue.emplace(item);
            }

            popCondition.notify_one();
        }

        void wait() {
            std::unique_lock<std::mutex> lock(this->popConditionMutex);
            popCondition.wait(lock, [this] {
                if (this->isClosed() == true || !queue.empty()) {
                    return true;
                }
                return false;
            });
        }

        T pop() {
       
            std::lock_guard<std::mutex> lockGuard(this->queueMutex);
            T item = queue.front();
            queue.pop();

            pushCondition.notify_one();

            return item;
        }

        void open() {
			this->close = false;
		}

        void closeNotify() {
            this->close = true;
            popCondition.notify_all();
            pushCondition.notify_all();
        }

        bool isClosed() {
            return this->close;
        }

        size_t size() const {
            return queue.size();
        }

    private:
        bool close;

        size_t maxSize;
        std::mutex queueMutex;
        std::queue<T> queue;

        std::mutex pushConditionMutex;
        std::mutex popConditionMutex;
        std::condition_variable pushCondition;
        std::condition_variable popCondition;
    };
}
