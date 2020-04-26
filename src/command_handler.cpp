#include "command_handler.h"
#include "commands.h"
#include "subscriber.h"
#include "subscriber_handler_creator.h"

command_handler::command_handler(
        std::unique_ptr<session_manager> sess_manager) :
    _sessions_manager(std::move(sess_manager)),
    _event_loop_thread(&command_handler::event_loop, this)
{
    subscribe(std::make_shared<subscriber>(1,
        subscriber_handler_creator::create_file_handler()));
    subscribe(std::make_shared<subscriber>(1,
        subscriber_handler_creator::create_console_handler()));
}

command_handler& command_handler::get_instance()
{
    static command_handler cmd_handler(std::make_unique<session_manager>());

    return cmd_handler;
}


void command_handler::add_request(const client_request request)
{
    if(_sessions_manager->session_exists(request.first))
        _clients_requests.push(std::move(request));
}

client_descriptor command_handler::add_client(size_t bulk_length)
{
    client_descriptor descriptor = _sessions_manager->create_session();

    auto start_command_ptr = commands_factory::create_control_command(command_type::start, bulk_length);
    _clients_requests.push(std::make_pair(descriptor, std::move(start_command_ptr)));

    return descriptor;
}

void command_handler::remove_client(client_descriptor descriptor)
{
    _sessions_manager->delete_session(descriptor);

    auto finish_command_ptr = commands_factory::create_control_command(command_type::finish);
    _clients_requests.push(std::make_pair(descriptor, std::move(finish_command_ptr)));
}


void command_handler::handle_command(client_descriptor descriptor,
        std::shared_ptr<base_command> command)
{
    switch (command->type()) {
        case command_type::open_scope: {
            handle_open_scope(descriptor);
            break;
        }
        case command_type::close_scope: {
            handle_close_scope(descriptor);
            break;
        }
        case command_type::text: {
            handle_text_command(descriptor,
                                command->timestamp(),
                                (dynamic_cast<text_command*>(command.get()))->info());
            break;
        }
        case command_type::finish : {
            handle_finish(descriptor);
            break;
        }
        case command_type::start : {
            handle_start(descriptor,
                         (dynamic_cast<start_command*>(command.get()))->bulk_length());
            break;
        }
    }
}


void command_handler::handle_open_scope(client_descriptor descriptor)
{
    auto context = _contexts[descriptor];
    if(context->current_scope_level > 0)
    {
        ++context->current_scope_level;
        return;
    }

    // отправить на выполнение команды нулевого scope
    // если он не пустой
    commands_description& commands_scope = context->commands[context->current_scope_level];

    if(!commands_scope.second.empty())
    {
        notify(commands_scope.first, commands_scope.second);
        commands_scope.second.clear();
    }

    //проинициализировать вложенный scope
    context->current_scope_level = 1;

    commands_scope = context->commands[context->current_scope_level];
    commands_scope.first = 0;
    commands_scope.second.clear();
}

void command_handler::handle_close_scope(client_descriptor descriptor)
{
    auto context = _contexts[descriptor];

    if(context->current_scope_level == 0)
    {
        std::string error = "error close scope";
        throw std::logic_error(error);
    }
    else if(context->current_scope_level == 1)
    {
        commands_description& commands = context->commands[context->current_scope_level];
        if(!commands.second.empty())
        {
            notify(commands.first, commands.second);
            commands.first = 0;
            commands.second.clear();
        }
    }

    --context->current_scope_level;
}

void command_handler::handle_finish(client_descriptor descriptor)
{
    auto context = _contexts[descriptor];

    if(context->current_scope_level == 0)
    {
        commands_description& commands = context->commands[context->current_scope_level];
        if(!commands.second.empty())
            notify(commands.first, commands.second);
    }

    _contexts.erase(descriptor);
}

void command_handler::handle_start(client_descriptor descriptor, size_t bulk_length)
{
    _contexts[descriptor] = std::make_shared<client_context>(bulk_length);
}

void command_handler::handle_text_command(client_descriptor descriptor, uint64_t timestamp, const std::string& str)
{
    auto context = _contexts[descriptor];
    size_t index = context->current_scope_level > 0 ? 1 : 0;
    commands_description& commands = context->commands[index];

    if(commands.second.empty())
        commands.first = timestamp;
    commands.second.push_back(str);

    if(context->current_scope_level == 0)
    {
        if(commands.second.size() == context->bulk_length)
        {
            notify(commands.first, commands.second);
            commands.second.clear();
        }
    }
}


void command_handler::subscribe(std::shared_ptr<base_subscriber> subscriber)
{
    _subscribers.push_back(subscriber);
}

void command_handler::notify(uint64_t timestamp,const scope_commands& cmds)
{
    auto task_ptr = std::make_shared<subscriber_task>(timestamp, cmds);

    for(auto subscriber : _subscribers)
    {
        auto shared_subscriber = subscriber.lock();
        if(shared_subscriber)
            shared_subscriber->update(task_ptr);
    }
}

void command_handler::event_loop()
{
    while(true)
    {
        auto request = _clients_requests.pop();
        handle_command(request.first, request.second);
    }
}
