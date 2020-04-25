#ifndef SUBSCRIBER_MOCK_H
#define SUBSCRIBER_MOCK_H

#include "base/base_subscriber.h"

struct subscriber_mock : base_subscriber {
    subscriber_mock() : _timestamp(0) {}

    void update(task_sptr task) override {
        _timestamp = task->timestamp;
        _output = task->commands;
    }

    const std::vector<std::string>& output() const {
        return  _output;
    }

    uint64_t timestamp() const {
        return _timestamp;
    }

private:
    std::vector<std::string> _output;
    uint64_t _timestamp;
};

#endif // SUBSCRIBER_MOCK_H
