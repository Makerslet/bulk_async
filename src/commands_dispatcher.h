#ifndef COMMANDS_DISPATCHER_H
#define COMMANDS_DISPATCHER_H

#include "base/base_control_command.h"
#include "client_descriptor.h"
#include "commands_queue.h"
#include "session_manager.h"
#include "command_handler.h"
#include "handler_context.h"

#include <unordered_map>
#include <memory>
#include <thread>


class commands_dispatcher
{
    using b_ctrl_cmd_sptr = std::shared_ptr<base_control_command>;
    using client_request = std::pair<client_descriptor, b_ctrl_cmd_sptr>;
    using handler_ctxt_uptr = std::unique_ptr<handler_context>;
    using subscriber_sptr = std::shared_ptr<base_subscriber>;
    using subscibers = std::vector<subscriber_sptr>;


public:
    static commands_dispatcher& get_instance();

    void handle_user_request(client_descriptor descriptor, const std::string &request);

    client_descriptor add_client(size_t bulk_length);

    void remove_client(client_descriptor descriptor);

private:
    /**
     * @brief Конструктор
     */
    commands_dispatcher();

    /**
     * @brief Конструктор копирования
     */
    commands_dispatcher(const commands_dispatcher&) = delete;

    /**
     * @brief Оператор присваивания
     */
    commands_dispatcher& operator=(const commands_dispatcher&) = delete;

    /**
     * @brief Внутренний цикл обработки комманд
     */
    void event_loop();

    /**
     * @brief Метод обработки команды начала ввода
     * @arg descriptor - дескрипток клиента
     * @arg bulk_length - длинна цепочки команд
     */
    void handle_start_control_command(client_descriptor descriptor, size_t bulk_length);

    /**
     * @brief Метод обработки команды завершения ввода
     * @arg descriptor - дескрипток клиента
     */
    void handle_stop_control_command(client_descriptor descriptor);

    /**
     * @brief Метод обработки текстовой последовательности от клиента
     * @arg descriptor - дескрипток клиента
     * @arg timestamp - временная метка
     * @arg text - текст комманд от пользователя
     */
    void handle_text_control_command(client_descriptor descriptor, uint64_t timestamp, const std::string& text);

    /**
     * @brief Метод обработки управляющей команды
     * @arg request - описание управляющей команды
     */
    void handle_control_command(client_request request);

private:
    std::unique_ptr<session_manager> _sessions_manager;
    std::thread _event_loop_thread;
    queue_mt<client_request> _clients_requests;

    std::unordered_map<client_descriptor, handler_ctxt_uptr> _handlers;
    subscibers _subscribers;
};

#endif // COMMANDS_DISPATCHER_H
