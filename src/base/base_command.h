#ifndef BASE_COMMAND_H
#define BASE_COMMAND_H

#include <cstdint>

/**
 * @brief Типы обрабатываемых команд
 */
enum class command_type {
    open_scope,
    close_scope,
    finish,
    text
};

/**
 * @brief Базовый тип команд
 */
struct base_command {
    /**
     * @brief Конструктор базовой команды
     * @param type - тип команды
     * @param timestamp - временная метка получения команды
     */
    base_command(command_type type, uint64_t timestamp) :
        _type(type), _timestamp(timestamp) {}

    /**
     * @brief Деструктор базовой команды
     */
    virtual ~base_command() = default;

    /**
     * @brief Геттер для типа команды
     * @return тип команды
     */
    command_type type() const {
        return _type;
    };

    /**
     * @brief Геттер временной метки создания команды
     * @return временная метка создания команды
     */
    uint64_t timestamp() const {
        return _timestamp;
    }

private:
    command_type _type;
    uint64_t _timestamp;
};

#endif // BASE_COMMAND_H
