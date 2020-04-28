#ifndef BASE_CONTROL_COMMAND_H
#define BASE_CONTROL_COMMAND_H

#include <cstdint>

/**
 * @brief Типы обрабатываемых управляющих команд
 */
enum class control_command_type
{
    start,
    stop,
    handle_text
};

/**
 * @brief Базовый тип управляющих команд
 */
struct base_control_command
{
    /**
     * @brief Конструктор базовой команды
     * @param type - тип команды
     * @param timestamp - временная метка получения команды
     */
    base_control_command(control_command_type type, uint64_t timestamp) :
        _type(type), _timestamp(timestamp)
    {}

    /**
     * @brief Деструктор базовой команды
     */
    virtual ~base_control_command() = default;

    /**
     * @brief Геттер для типа команды
     * @return тип команды
     */
    control_command_type type() const
    {
        return _type;
    };

    /**
     * @brief Геттер временной метки создания команды
     * @return временная метка создания команды
     */
    uint64_t timestamp() const
    {
        return _timestamp;
    }

private:
    control_command_type _type;
    uint64_t _timestamp;
};


#endif // BASE_CONTROL_COMMAND_H
