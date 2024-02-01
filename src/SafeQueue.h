//
// Created by jkshi on 01/02/2024.
//

#ifndef VOICETRANSCRIPTIONAPP_SAFEQUEUE_H
#define VOICETRANSCRIPTIONAPP_SAFEQUEUE_H
#include <queue>
#include <mutex>
#include <optional>

template<typename T>
class SafeQueue {
private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;

    bool empty() const {
        return queue_.empty();
    }
public:
    SafeQueue() = default;
    SafeQueue(const SafeQueue<T> &) = delete ;
    SafeQueue& operator=(const SafeQueue<T> &) = delete ;

    SafeQueue(SafeQueue<T>&& other) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_ = std::move(other.queue_);
    }
    virtual ~SafeQueue() {}

    unsigned long size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    std::optional<T> pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return {};
        }
        T tmp = queue_.front();
        queue_.pop();
        return tmp;
    }

    void push(const T &value) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(value);
    }
};


#endif //VOICETRANSCRIPTIONAPP_SAFEQUEUE_H
