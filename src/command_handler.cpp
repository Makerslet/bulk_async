#include "command_handler.h"
#include "commands.h"

command_handler::command_handler(std::size_t bulk_length) :
    _bulk_length(bulk_length), _current_scope_level(0)
{
    _commands.emplace_back(commands_description());
    _commands.emplace_back(commands_description());
}

const command_handler_statistic& command_handler::statistic() const
{
    return _statistic;
}

void command_handler::add_command(std::unique_ptr<base_command>&& command)
{
    switch (command->type()) {
        case command_type::open_scope: {
            handle_open_scope();
            break;
        }
        case command_type::close_scope: {
            handle_close_scope();
            break;
        }
        case command_type::text: {
            handle_text_command(command->timestamp(),
                                (dynamic_cast<text_command*>(command.get()))->info());
            break;
        }
    }
}


void command_handler::handle_open_scope()
{
    ++_statistic.num_lines;

    if(_current_scope_level > 0)
    {
        ++_current_scope_level;
        return;
    }

    // отправить на выполнение команды нулевого scope
    // если он не пустой
    commands_description& commands_scope = _commands[_current_scope_level];

    if(!commands_scope.second.empty())
    {
        notify(commands_scope.first, commands_scope.second);
        commands_scope.second.clear();
    }

    //проинициализировать вложенный scope
    _current_scope_level = 1;

    commands_scope = _commands[_current_scope_level];
    commands_scope.first = 0;
    commands_scope.second.clear();
}

void command_handler::handle_close_scope()
{
    ++_statistic.num_lines;

    if(_current_scope_level == 0)
    {
        throw std::logic_error(
                    "You can't use close scope operator out of scope. "
                    "This command will be ignored."
                    );
    }
    else if(_current_scope_level == 1)
    {
        commands_description& commands = _commands[_current_scope_level];
        if(!commands.second.empty())
        {
            notify(commands.first, commands.second);
            commands.first = 0;
            commands.second.clear();
        }
    }

    --_current_scope_level;
}

void command_handler::stop_handling()
{
    if(_current_scope_level == 0)
    {
        commands_description& commands = _commands[_current_scope_level];
        if(!commands.second.empty())
            notify(commands.first, commands.second);
    }
}

void command_handler::handle_text_command(uint64_t timestamp, const std::string& str)
{
    ++_statistic.num_lines;

    size_t index = _current_scope_level > 0 ? 1 : 0;
    commands_description& commands = _commands[index];

    if(commands.second.empty())
        commands.first = timestamp;
    commands.second.push_back(str);

    if(_current_scope_level == 0)
    {
        if(commands.second.size() == _bulk_length)
        {
            notify(commands.first, commands.second);
            commands.second.clear();
        }
    }
}

void command_handler::notify(uint64_t timestamp,const scope_commands& cmds)
{
    ++_statistic.num_blocks;
    _statistic.num_commands += cmds.size();

    auto task_ptr = std::make_shared<subscriber_task>(timestamp, cmds);

    for(auto subscriber : _subscribers)
    {
        auto shared_subscriber = subscriber.lock();
        if(shared_subscriber)
            shared_subscriber->update(task_ptr);
    }
}
