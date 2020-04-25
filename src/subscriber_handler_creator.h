#ifndef SUBSCRIBER_HANDLER_CREATOR_H
#define SUBSCRIBER_HANDLER_CREATOR_H

#include "base/base_subscriber.h"

/**
 * @brief Генерато обработчиков задач для worker'ов
 */
class subscriber_handler_creator
{
public:
    /**
     * @brief Генерация обработчика вывода задач в консоль
     * @return Обработчик вывода задач в консоль
     */
    static subscriber_task_handler create_console_handler();
    /**
     * @brief Генерация обработчика вывода задач в файл
     * @return Обработчик вывода задач в файл
     */
    static subscriber_task_handler create_file_handler();
};

#endif // SUBSCRIBER_HANDLER_CREATOR_H
