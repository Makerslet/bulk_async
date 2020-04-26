#ifndef COMMANDS_FACTORY_H
#define COMMANDS_FACTORY_H

#include "base/base_command.h"

#include <memory>
/**
 * @brief Фабрика команд
 */
class commands_factory
{
public:
    static std::unique_ptr<base_command> create_control_command(command_type type, size_t arg = 0);
    /**
     * @brief Метод создания команд
     * @param str - текст
     * @return указатель на команду
     */
    static std::unique_ptr<base_command> create_user_command(const std::string& str);
private:
    /**
     * @brief Метод получения временной метки
     * @return временная метка
     */
    static uint64_t create_time_stamp();
};

#endif // COMMANDS_FACTORY_H
