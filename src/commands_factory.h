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
    /**
     * @brief Метод создания команд
     * @param str - текст
     * @return указатель на команду
     */
    static std::unique_ptr<base_command> create_user_command(uint64_t timestamp, const std::string& str);
};

#endif // COMMANDS_FACTORY_H
