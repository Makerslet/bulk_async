#include "commands_dispatcher.h"
#include "subscriber.h"
#include "subscriber_handler_creator.h"
#include "control_commands.h"
#include "util.h"

commands_dispatcher::commands_dispatcher() :
    _sessions_manager(std::make_unique<session_manager>()),
    _event_loop_thread(&commands_dispatcher::event_loop, this)
{
    _event_loop_thread.detach();

    _subscribers.emplace_back(
                std::make_shared<subscriber>(1,
                        subscriber_handler_creator::create_console_handler()));
    _subscribers.emplace_back(
                std::make_shared<subscriber>(1,
                        subscriber_handler_creator::create_file_handler()));
}

commands_dispatcher& commands_dispatcher::get_instance()
{
    static commands_dispatcher cmd_dispatcher;
    return cmd_dispatcher;
}

void commands_dispatcher::handle_user_request(client_descriptor descriptor, const std::string &request)
{
    if(!_sessions_manager->session_exists(descriptor))
        return;

    auto text_cmd = std::make_shared<text_control_command>(util::create_time_stamp(), request);
    _clients_requests.push(std::make_pair(descriptor, text_cmd));
}

client_descriptor commands_dispatcher::add_client(size_t bulk_length)
{
    client_descriptor descriptor = _sessions_manager->create_session();

    auto start_cmd = std::make_shared<start_control_command>(util::create_time_stamp(), bulk_length);
    _clients_requests.push(std::make_pair(descriptor, start_cmd));

    return descriptor;
}

void commands_dispatcher::remove_client(client_descriptor descriptor)
{
    _sessions_manager->delete_session(descriptor);

    auto stop_cmd = std::make_shared<stop_control_command>(util::create_time_stamp());
    _clients_requests.push(std::make_pair(descriptor, stop_cmd));
}


void commands_dispatcher::event_loop()
{
    while(true)
    {
        auto request = _clients_requests.pop();
        handle_control_command(request);
    }
}

void commands_dispatcher::handle_start_control_command(client_descriptor descriptor, size_t bulk_length)
{
    auto handler_ctxt = std::make_unique<handler_context>(bulk_length);

    for(auto& subscriber : _subscribers)
        handler_ctxt->handler.subscribe(subscriber);

    _handlers[descriptor] = std::move(handler_ctxt);
}

void commands_dispatcher::handle_stop_control_command(client_descriptor descriptor)
{
    auto& context = _handlers[descriptor];
    context->handler.stop_handling();
}

void commands_dispatcher::handle_text_control_command(client_descriptor descriptor, uint64_t timestamp, const std::string& text)
{
    auto& context = _handlers[descriptor];
    context->buffer.push_back(std::make_pair(timestamp, text));
    context->process_buffer();
}

void commands_dispatcher::handle_control_command(client_request request)
{
    auto descriptor = request.first;
    std::shared_ptr<base_control_command> cmd = request.second;

    switch (cmd->type()) {
        case control_command_type::start : {
            size_t bulk_length = dynamic_cast<start_control_command*>(cmd.get())->bulk_length();
            handle_start_control_command(descriptor, bulk_length);
            break;
        }
        case control_command_type::stop:
            handle_stop_control_command(descriptor);
            break;
        case control_command_type::handle_text : {
            std::string text = dynamic_cast<text_control_command*>(cmd.get())->text();
            handle_text_control_command(descriptor, cmd->timestamp(), text);
            break;
        }
    }
}
