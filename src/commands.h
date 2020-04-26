#ifndef COMMANDS_H
#define COMMANDS_H

#include "base/base_command.h"

#include <string>

/**
 * @brief Команда открытия scope
 */
struct open_scope_command : base_command {
    /**
     * @brief Конструктор
     * @param ts - временная метка
     */
    open_scope_command(uint64_t ts) :
        base_command(command_type::open_scope, ts) {}
};

/**
 * @brief Команда закрытия scope
 */
struct close_scope_command : base_command {
    /**
     * @brief Конструктор
     * @param ts - временная метка
     */
    close_scope_command(uint64_t ts) :
        base_command(command_type::close_scope, ts) {}
};


/**
 * @brief Текстовая команда
 */
struct text_command : base_command {
    /**
     * @brief Конструктор
     * @param ts - временная метка
     * @param cmd - текст комманды
     */
    text_command(uint64_t ts, const std::string& cmd) :
        base_command(command_type::text, ts),
        _info(cmd) {}

    /**
     * @brief Метод получения текста команды
     * @return текст команды
     */
    const std::string& info() const {
        return _info;
    }
private:
    std::string _info;
};


/**
 * @brief Команда завершения ввода
 */
struct finish_command : base_command {
    /**
     * @brief Конструктор
     * @param ts - временная метка
     */
    finish_command(uint64_t ts) :
        base_command(command_type::finish, ts) {}
};


/**
 * @brief Команда начала работы
 */
struct start_command : base_command {
    /**
     * @brief Конструктор
     * @param ts - временная метка
     */
    start_command(uint64_t ts, size_t bulk_length) :
        base_command(command_type::start, ts), _bulk_length(bulk_length) {}

    size_t bulk_length() const {
        return _bulk_length;
    }

private:
    size_t _bulk_length;
};

#endif // COMMANDS_H
