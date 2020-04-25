#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>

/**
 * @brief Класс полезных служебных функций
 */
struct util
{
    using commands = std::vector<std::string>;
    /**
     * @brief Преобразование набора команд в строковый вид
     * @arg cmmds - набор команд
     * @return Строковое представление
     */
    static std::string prepare_string_command(const commands& cmmds);
};

#endif // UTIL_H
