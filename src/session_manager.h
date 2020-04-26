#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "client_descriptor.h"

#include <mutex>
#include <set>

class session_manager
{
public:
    session_manager();

    client_descriptor create_session();

    bool session_exists(client_descriptor descriptor);

    void delete_session(client_descriptor descriptor);

private:
    std::mutex _guard;
    std::set<client_descriptor> _sessions;
};

#endif // SESSION_MANAGER_H
