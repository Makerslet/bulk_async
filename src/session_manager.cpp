#include "session_manager.h"

session_manager::session_manager()
{

}

client_descriptor session_manager::create_session()
{
    client_descriptor descriptor;
    if(_sessions.empty())
        descriptor = 0;
    else
    {
        auto max_value = *(--_sessions.end());
        descriptor = ++max_value;
    }

    _sessions.insert(descriptor);
    return descriptor;
}

bool session_manager::session_exists(client_descriptor descriptor)
{
    return _sessions.find(descriptor) != _sessions.end();
}

void session_manager::delete_session(client_descriptor descriptor)
{
    _sessions.erase(descriptor);
}
