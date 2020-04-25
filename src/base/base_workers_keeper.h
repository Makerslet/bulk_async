#ifndef BASE_WORKERS_KEEPER_H
#define BASE_WORKERS_KEEPER_H

#include <functional>
#include <atomic>
#include <memory>
#include <condition_variable>
#include <queue>

/**
 * @brief Задача, которая крутится внутри потока
 */
using worker_task = std::function<void()>;

/**
 * @brief Описание контекста worker'а,
 * который содержит необходимую статистику
 */
struct worker_context
{
    /**
     * @brief Конструктор контекста worker'a
     * @arg name имя worker'a
     */
    worker_context(const std::string& name) :
        name(name)
    {
        num_blocks.store(0);
        num_commands.store(0);
    }

    /**
     * @brief Число блоков обработанных worker'ом
     */
    std::atomic<size_t> num_blocks;
    /**
     * @brief Число комманд, обработанных worker'ом
     */
    std::atomic<size_t> num_commands;
    /**
     * @brief Имя worker'а
     */
    const std::string name;
};

using context_sptr = std::shared_ptr<worker_context>;

/**
 * @brief Базовый класс поддержки worker'ов
 * для исполнения задач
 */
class base_workers_keeper
{
public:
    virtual ~base_workers_keeper() = default;

    virtual size_t num_workers() const = 0;
    virtual context_sptr get_worker_context(size_t worker_index) = 0;
};

#endif // BASE_WORKERS_KEEPER_H
