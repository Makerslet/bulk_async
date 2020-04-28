#ifndef CONTROL_COMMANDS_H
#define CONTROL_COMMANDS_H

#include "base/base_control_command.h"

#include <string>

/**
 * @brief Команда начала работы с пользователем
 */
struct start_control_command : base_control_command
{
    /**
     * @brief Конструктор
     * @arg ts - временная метка
     * @arg bulk_length - длина цепочки команд
     */
    start_control_command(uint64_t ts, size_t bulk_length) :
        base_control_command(control_command_type::start, ts),
        _bulk_length(bulk_length) {}

    /**
     * @brief Получение длины цепочки команд
     * @return Длина цепочки команд
     */
    size_t bulk_length() const
    {
        return _bulk_length;
    }

private:
    size_t _bulk_length;
};

/**
 * @brief Команда окончания работы с пользователем
 */
struct stop_control_command : base_control_command
{
    /**
     * @brief Конструктор
     * @arg ts - временная метка
     */
    stop_control_command(uint64_t ts) :
        base_control_command(control_command_type::stop, ts) {}
};


/**
 * @brief Текстовая обработки поступивших текстовых данных
 */
struct text_control_command : base_control_command
{
    /**
     * @brief Конструктор
     * @arg ts - временная метка
     * @arg cmd - текст
     */
    text_control_command(uint64_t ts, const std::string& cmd) :
        base_control_command(control_command_type::handle_text, ts),
        _info(cmd) {}

    /**
     * @brief Метод получения текста
     * @return текст команды
     */
    const std::string& text() const
    {
        return _info;
    }
private:
    std::string _info;
};

#endif // CONTROL_COMMANDS_H
