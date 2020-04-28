#ifndef HANDLER_CONTEXT_H
#define HANDLER_CONTEXT_H

#include "command_handler.h"

#include <list>

/**
 * @brief Контекст команд обрабатываемых от одного клиента
 */
struct handler_context
{
    using buffer_item = std::pair<uint64_t, std::string>;

    /**
     * @brief Конструктор
     * @arg bulk_length - длина цепочки команд
     */
    handler_context(size_t bulk_length);

    /**
     * @brief Обработка накопившегося буффера
     */
    void process_buffer();

    std::list<buffer_item> buffer;
    command_handler handler;

private:
    /**
     * @brief Слияние одной записи с другими в буфере
     * @arg item - запись в буфере
     */
    void merge_in_buffer(const buffer_item& item);

    /**
     * @brief Извлечение из строки комманд и передача их на обработку
     * @arg ts - временная метка
     * @arg str - строка
     */
    void create_and_add_commands(uint64_t ts, const std::string& str);
};
#endif // HANDLER_CONTEXT_H
