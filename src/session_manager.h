#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "client_descriptor.h"

#include <mutex>
#include <set>

/**
 * @brief Менеджер управления сессиями клиентов
 */
class session_manager
{
public:
    /**
     * @brief Конструктор
     */
    session_manager();

    /**
     * @brief Создание сессии
     * @return дескриптор клиента
     */
    client_descriptor create_session();

    /**
     * @brief Проверка существования сесси
     * @arg descriptor - дескриптор клиента
     * @return Существует ли сессия
     */
    bool session_exists(client_descriptor descriptor);

    /**
     * @brief Удалить сессию
     * @arg descriptor - дескриптор клиента
     */
    void delete_session(client_descriptor descriptor);

private:
    std::mutex _guard;
    std::set<client_descriptor> _sessions;
};

#endif // SESSION_MANAGER_H
