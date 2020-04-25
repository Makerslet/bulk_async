#include "subscriber_handler_creator.h"
#include "util.h"

#include <chrono>
#include <fstream>
#include <iostream>

subscriber_task_handler subscriber_handler_creator::create_console_handler()
{
    return [](task_sptr task, const std::string&) {
        std::cout << util::prepare_string_command(task->commands) << std::endl;
    };
}

subscriber_task_handler subscriber_handler_creator::create_file_handler()
{
    return [](task_sptr task, const std::string& name) {
        std::string file_name = name + ".log";
        std::ofstream fstream(file_name);
        if(!fstream.is_open()) {
            std::string error = "can't open file " + file_name;
            throw std::runtime_error(error);
        }

        fstream << util::prepare_string_command(task->commands) << std::endl;
        fstream.close();
    };
}
