#include "async.h"
#include "commands_dispatcher.h"

namespace async {

handle_t connect(std::size_t bulk_length)
{
    client_descriptor descriptor = commands_dispatcher::get_instance().add_client(bulk_length);
    return reinterpret_cast<handle_t>(descriptor);
}

void receive(handle_t handle, const char* str, std::size_t size)
{
    commands_dispatcher& handler = commands_dispatcher::get_instance();
    client_descriptor descriptor = reinterpret_cast<client_descriptor>(handle);

    handler.handle_user_request(descriptor, std::string(str, size));
}

void disconnect(handle_t handle)
{
    commands_dispatcher& handler = commands_dispatcher::get_instance();
    client_descriptor descriptor = reinterpret_cast<client_descriptor>(handle);

    handler.remove_client(descriptor);
}

}
