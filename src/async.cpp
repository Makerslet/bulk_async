#include "async.h"
#include "command_handler.h"
#include "requests_parser.h"

namespace async {

handle_t connect(std::size_t bulk_length)
{
    client_descriptor descriptor = command_handler::get_instance().add_client(bulk_length);
    return reinterpret_cast<handle_t>(descriptor);
}

void receive(handle_t handle, const char* str, std::size_t size)
{
    command_handler& handler = command_handler::get_instance();
    client_descriptor descriptor = reinterpret_cast<client_descriptor>(handle);

    std::string requests(str, size);
    for(auto command : requests_parser::parse_requests(requests))
        handler.add_request(std::make_pair(descriptor, command));
}

void disconnect(handle_t handle)
{
    command_handler& handler = command_handler::get_instance();
    client_descriptor descriptor = reinterpret_cast<client_descriptor>(handle);

    handler.remove_client(descriptor);
}

}
