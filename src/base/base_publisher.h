#ifndef BASE_PUBLISHER_H
#define BASE_PUBLISHER_H

#include "base_subscriber.h"

#include <memory>

/**
 * @brief Интерфейс publisher'а
 */
struct base_publisher
{

    /**
     * @brief Деструктор publisher'а
     */
    virtual ~base_publisher() = default;

    /**
     * @brief Метод подписки на уведомления
     * @param subscriber - подписчик
     */
    void subscribe(std::shared_ptr<base_subscriber> subscriber)
    {
        _subscribers.push_back(subscriber);
    }

protected:
    std::vector<std::weak_ptr<base_subscriber>> _subscribers;
};

#endif // BASE_PUBLISHER_H
