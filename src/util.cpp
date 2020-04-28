#include "util.h"

#include <chrono>

std::string util::prepare_string_command(const commands& cmds)
{
    std::string result("bulk: ");
    for(const auto& command : cmds)
        result += command + ", ";

    result.erase(result.length() - 2, 2);
    return result;
}

uint64_t util::create_time_stamp()
{
    auto now = std::chrono::system_clock::now();
    return  std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
}
