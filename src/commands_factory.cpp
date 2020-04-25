#include "commands_factory.h"
#include "commands.h"

#include <chrono>

std::unique_ptr<base_command> commands_factory::create_command(const std::string& str)
{
    uint64_t timestamp = create_time_stamp();

    if(str == "{")
        return std::make_unique<open_scope_command>(timestamp);
    else if(str == "}")
        return  std::make_unique<close_scope_command>(timestamp);
    else if(str == "exit")
        return  std::make_unique<finish_command>(timestamp);
    else
        return std::make_unique<text_command>(timestamp, str);
}

uint64_t commands_factory::create_time_stamp()
{
    auto now = std::chrono::system_clock::now();
    return  std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
}
