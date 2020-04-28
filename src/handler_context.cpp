#include "handler_context.h"
#include "commands_factory.h"

#include <sstream>

handler_context::handler_context(size_t bulk_length) :
    handler(bulk_length)
{}

void handler_context::process_buffer()
{
    while(!buffer.empty())
    {
        auto front_elem = buffer.front();
        buffer.pop_front();

        // ищем последний разделитель
        auto position = front_elem.second.find_last_of('\n');

        //если не нашли то пытаемся вернуть блок назад или смержить со следующим
        if(position == std::string::npos)
        {
            bool need_stop = buffer.empty();
            merge_in_buffer(front_elem);
            if(need_stop)
                break;
        }
        else
        {
            // нашли последовательность комманд, отправляем их на обработку
            std::string str = front_elem.second.substr(0, position + 1);
            create_and_add_commands(front_elem.first, str);
            front_elem.second.erase(0, position + 1);

            // недозаполненные команды пытаемся вернуть назад или смержить
            if(!front_elem.second.empty())
            {
                bool need_stop = buffer.empty();
                merge_in_buffer(front_elem);
                if(need_stop)
                    break;
            }
        }
    }
}

void handler_context::create_and_add_commands(uint64_t ts, const std::string& str)
{
    std::vector<std::string> string_cmds;
    std::stringstream ss(str);
    std::string req;
    while(std::getline(ss, req, '\n')) {
        string_cmds.push_back(req);
    }

    for(const std::string& cmd_str : string_cmds)
        handler.add_command(
                    commands_factory::create_user_command(ts, cmd_str));
}

void handler_context::merge_in_buffer(const buffer_item& item)
{
    if(buffer.empty())
        buffer.push_front(item);
    else
        buffer.front().second.insert(0, item.second);
}
