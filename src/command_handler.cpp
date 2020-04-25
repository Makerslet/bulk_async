#include "command_handler.h"
#include "commands.h"
#include "subscriber.h"
#include "subscriber_handler_creator.h"

#include <boost/algorithm/string/split.hpp>

command_handler::command_handler(
        std::unique_ptr<commands_factory> factory,
        std::unique_ptr<session_manager> sess_manager) :
    _commands_factory(std::move(factory)),
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
    static command_handler cmd_handler(
                std::make_unique<commands_factory>(),
                std::make_unique<session_manager>());

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
    _contexts[descriptor] = std::make_shared<client_context>(bulk_length);
    return descriptor;
}

void command_handler::remove_client(client_descriptor descriptor)
{
    _sessions_manager->delete_session(descriptor);
    _contexts.erase(descriptor);
}


void command_handler::add_command(client_descriptor descriptor,
        std::unique_ptr<base_command>&& command)
{
    auto& context = _contexts[descriptor];

    switch (command->type()) {
        case command_type::open_scope: {
            handle_open_scope(context);
            break;
        }
        case command_type::close_scope: {
            handle_close_scope(context);
            break;
        }
        case command_type::finish : {
            handle_finish(context);
            break;
        }
        case command_type::text: {
            handle_text_command(context,
                                command->timestamp(),
                                (dynamic_cast<text_command*>(command.get()))->info());
            break;
        }
    }
}


void command_handler::handle_open_scope(client_context_sptr context)
{
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

void command_handler::handle_close_scope(client_context_sptr context)
{
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

void command_handler::handle_finish(client_context_sptr context)
{
    if(context->current_scope_level == 0)
    {
        commands_description& commands = context->commands[context->current_scope_level];
        if(!commands.second.empty())
            notify(commands.first, commands.second);
    }
}

void command_handler::handle_text_command(client_context_sptr context, uint64_t timestamp, const std::string& str)
{
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

        std::vector<std::string> string_cmds;
        boost::split(string_cmds, request.second, [](char s) {return s == '\n';});

        for(const std::string& cmd : string_cmds)
            add_command(request.first,
                        _commands_factory->create_command(cmd));
    }
}
