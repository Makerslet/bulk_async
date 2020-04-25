#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "base/base_command.h"
#include "base/base_publisher.h"
#include "client_descriptor.h"
#include "commands_queue.h"
#include "commands_factory.h"
#include "session_manager.h"

#include <memory>
#include <map>
#include <thread>

/**
 * @brief Класс обработки команд
 */
class command_handler : public base_publisher
{
    /**
     * @brief Команды принадлежащие определенному scope
     */
    using scope_commands = std::vector<std::string>;
    /**
     * @brief Временная метка первой команды с scope и список команд
     */
    using commands_description = std::pair<uint64_t, scope_commands>;

    using client_request = std::pair<client_descriptor, std::string>;

    struct client_context
    {
        client_context(size_t bulk_length) :
            bulk_length(bulk_length),
            current_scope_level(0)
        {
            commands.emplace_back(commands_description());
            commands.emplace_back(commands_description());
        }

        std::size_t bulk_length;
        std::size_t current_scope_level;
        std::vector<commands_description> commands;
    };

    using client_context_sptr = std::shared_ptr<client_context>;

public:
    command_handler& get_instance();

    void add_request(const client_request request);

    client_descriptor add_client(size_t bulk_length);

    void remove_client(client_descriptor descriptor);

    /**
     * @brief Метод запуска команды в обработку
     * @param command - команда
     */
    void add_command(client_descriptor descriptor, std::unique_ptr<base_command>&& command);

    /**
     * @brief Метод подписки на обновлениия
     * @param subscriber - подписчик
     */
    void subscribe(std::shared_ptr<base_subscriber> subscriber) override;

private:

    /**
     * @brief Конструктор обработчика команд
     * @param bulk_length - размер пакета команд
     */
    command_handler(
            std::unique_ptr<commands_factory> factory,
            std::unique_ptr<session_manager> sess_manager);

    void event_loop();
    /**
     * @brief Метод обработки оповещения подписчиков
     * @param timestamp - временная метка первой команды
     * @param string - информационная строка
     */
    void notify(uint64_t timestamp, const scope_commands &cmds);
    /**
     * @brief Метод обработки команды открытия scope
     */
    void handle_open_scope(client_context_sptr);
    /**
     * @brief Метод обработки команды закрытия scope
     */
    void handle_close_scope(client_context_sptr);
    /**
     * @brief Метод обработки команды завершения ввода
     */
    void handle_finish(client_context_sptr);
    /**
     * @brief Метод обработки текстовой команды
     * @param timestamp - временная метка
     * @param str - текст команды
     */
    void handle_text_command(client_context_sptr context, uint64_t timestamp, const std::string& str);

private:
    std::unique_ptr<commands_factory> _commands_factory;
    std::unique_ptr<session_manager> _sessions_manager;

    std::unordered_map<client_descriptor, client_context_sptr> _contexts;
    std::thread _event_loop_thread;
    queue_mt<client_request> _clients_requests;

    std::vector<std::weak_ptr<base_subscriber>> _subscribers;
};

#endif // COMMAND_HANDLER_H
