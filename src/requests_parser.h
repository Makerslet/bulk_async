#ifndef REQUESTS_PARSER_H
#define REQUESTS_PARSER_H

#include "base/base_command.h"

#include <memory>
#include <vector>

class requests_parser
{
public:
    static std::vector<std::shared_ptr<base_command>> parse_requests(const std::string& requests);
};

#endif // REQUESTS_PARSER_H
