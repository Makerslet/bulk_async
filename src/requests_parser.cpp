#include "requests_parser.h"
#include "commands_factory.h"

#include <boost/algorithm/string/split.hpp>

std::vector<std::shared_ptr<base_command>> requests_parser::parse_requests(
        const std::string& requests)
{
    std::vector<std::string> string_cmds;
    boost::split(string_cmds, requests, [](char s) {return s == '\n';});

    std::vector<std::shared_ptr<base_command>> commands;
    for(const std::string& cmd_str : string_cmds)
        commands.emplace_back(commands_factory::create_user_command(cmd_str));

    return commands;
}
