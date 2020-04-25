#ifndef BASE_SUBSCRIBER_H
#define BASE_SUBSCRIBER_H

#include <vector>
#include <string>
#include <memory>
#include <functional>

/**
 * @brief Описание задачи для вывода
 */
struct subscriber_task {
    subscriber_task(uint64_t ts, const std::vector<std::string>& cmds) :
        timestamp(ts), commands(cmds){}

    const uint64_t timestamp;
    const std::vector<std::string> commands;
};

using task_sptr = std::shared_ptr<subscriber_task>;
using subscriber_task_handler = std::function<void(task_sptr, const std::string&)>;

/**
 * @brief Интерфейс подписчика
 */
struct base_subscriber {
    /**
     * @brief Деструктор подписчика
     */
    virtual ~base_subscriber() = default;
    /**
     * @brief Интерфейс получения уведомлений
     * @timestamp - временная метка
     * @str - информационная строка
     */
    virtual void update(task_sptr) = 0;
};

#endif // BASE_SUBSCRIBER_H
