#include "subscriber.h"

#include <sstream>

subscriber::subscriber(size_t num_workers, subscriber_task_handler task) :
    _num_workers(num_workers),
    _cmds_queue(std::make_shared<queue_mt<task_sptr>>())
{
    if(_num_workers == 0)
        return;

    for(size_t i = 0; i < _num_workers; ++i)
    {
        auto context = std::make_shared<worker_context>("th" + std::to_string(i));
        worker_task t = create_worker_handler(context, _cmds_queue, task);

        _workers.emplace_back(std::make_pair(context, std::thread(t)));
        _workers[i].second.detach();
    }
}

void subscriber::update(task_sptr task)
{
    _cmds_queue->push(task);
}


size_t subscriber::num_workers() const
{
    return _num_workers;
}

context_sptr subscriber::get_worker_context(size_t worker_index)
{
    context_sptr context = nullptr;

    if(worker_index < _workers.size())
        context = _workers[worker_index].first;

    return context;
}

worker_task subscriber::create_worker_handler(context_sptr context, queue_sptr queue,
                               subscriber_task_handler task)
{
    return [context, queue, task]() {
        while(true)
        {
            task_sptr command = queue->pop();
            context->num_blocks += 1;
            context->num_commands += command->commands.size();

            std::stringstream ss;
            ss << command->timestamp << context->name << context->num_blocks;

            task(command, ss.str());
        }
    };
}
