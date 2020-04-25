#include "util.h"

std::string util::prepare_string_command(const commands& cmds)
{
    std::string result("bulk: ");
    for(const auto& command : cmds)
        result += command + ", ";

    result.erase(result.length() - 2, 2);
    return result;
}
