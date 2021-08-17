#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <stdexcept>

class QueueException: public std::runtime_error {
    public:
        QueueException(const char* msg): runtime_error(msg) {}
};

class QueueFullException: public QueueException {
    public:
        QueueFullException(): QueueException("queue full") {}
};

template <class T>
class BlockingQueue: public std::queue<T*> {
    public:
        explicit BlockingQueue(int size);
        ~BlockingQueue();
        void push(T* item, bool block = true);
        bool full();
        bool empty();
        T* pop();

    private:
        int maxSize;
        bool run = true;
        std::mutex queueMutex;
        std::condition_variable isFull;
        std::condition_variable isEmpty;
};
