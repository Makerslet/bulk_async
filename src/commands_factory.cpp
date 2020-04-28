#include "commands_factory.h"
#include "commands.h"

std::unique_ptr<base_command> commands_factory::create_user_command(uint64_t timestamp, const std::string& str)
{
    if(str == "{")
        return std::make_unique<open_scope_command>(timestamp);
    else if(str == "}")
        return  std::make_unique<close_scope_command>(timestamp);
    else
        return std::make_unique<text_command>(timestamp, str);
}

