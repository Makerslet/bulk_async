#ifndef COMMANDS_QUEUE_H
#define COMMANDS_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

/**
 * @brief Класс многопоточной неограниченной очереди,
 * обеспечивающей ожидание и уведомление читающих потоков
 */
template<typename T>
class queue_mt
{
public:
    /**
     * @brief Конструктор
     */
    queue_mt()
    {}

    /**
     * @brief Положить элемен в очередь
     */
    void push(const T& value)
    {
        std::unique_lock<std::mutex> lock(_control_mutex);
        _cmds_queue.push(value);
        lock.unlock();
        _cond_var.notify_one();
    }

    /**
     * @brief Извлечь элемент из очереди
     */
    T pop()
    {
        std::unique_lock<std::mutex> lock(_control_mutex);
        _cond_var.wait(lock, [this](){return !_cmds_queue.empty();});

        T value = _cmds_queue.front();
        _cmds_queue.pop();

        return value;
    }

private:
    std::condition_variable _cond_var;
    std::mutex _control_mutex;
    std::queue<T> _cmds_queue;
};

#endif // COMMANDS_QUEUE_H
