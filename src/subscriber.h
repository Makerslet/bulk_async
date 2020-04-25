#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include "commands_queue.h"

#include "base/base_subscriber.h"
#include "base/base_workers_keeper.h"

#include <thread>

/**
 * @brief Обобщенный класс подписчика, реализующий обработку комманд
 * внутри потоков-worker'ов
 */
class subscriber : public base_subscriber, public base_workers_keeper
{
    /**
     * @brief Пара контекст worker'a - поток worker'а
     */
    using context_thread_pair = std::pair<context_sptr, std::thread>;
    /**
     * @brief Синоним указателя на многопоточную очередь команд
     */
    using queue_sptr = std::shared_ptr<queue_mt<task_sptr>>;

public:
    /**
     * @brief Конструктор
     * @arg num_workers - число требущихся worker'ов
     * @arg subscriber_task - обработчик, который будут выполнять worker'ы
     */
    subscriber(size_t num_workers, subscriber_task_handler subscriber_task);
    /**
     * @brief Обработка команды от publisher'а
     * @param cmds - набор комманд для обработки
     */
    void update(task_sptr cmds) override;

    /**
     * @brief Число worker'ов работающих внутри подписчика
     * @return Число worker'ов
     */
    size_t num_workers() const override;

    /**
     * @brief Получение контекста worker'а
     * @arg worker_index - индекс воркера
     * @return Контекст worker'а
     */
    context_sptr get_worker_context(size_t worker_index) override;

private:
    /**
     * @brief Метод записи в файл
     * @param timestamp - временная метка
     * @param out - текст
     */
    worker_task create_worker_handler(context_sptr context, queue_sptr queue_mt,
                                   subscriber_task_handler subscriber_task);

private:
    size_t _num_workers;
    queue_sptr _cmds_queue;
    std::vector<context_thread_pair> _workers;
};

#endif // SUBSCRIBER_H
