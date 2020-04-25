#include "subscriber_mock.h"
#include "commands_factory.h"
#include "command_handler.h"
#include "commands.h"

#include <gtest/gtest.h>

TEST(FACTORY, OPEN_SCOPE_COMMAND)
{
    std::string str = "{";
    commands_factory factory;
    auto cmd = factory.create_command(str);

    ASSERT_EQ(cmd->type(), command_type::open_scope);
}

TEST(FACTORY, CLOSE_SCOPE_COMMAND)
{
    std::string str = "}";
    commands_factory factory;
    auto cmd = factory.create_command(str);

    ASSERT_EQ(cmd->type(), command_type::close_scope);
}

TEST(FACTORY, FINISH_COMMAND)
{
    std::string str = "exit";
    commands_factory factory;
    auto cmd = factory.create_command(str);

    ASSERT_EQ(cmd->type(), command_type::finish);
}

TEST(FACTORY, TEXT_COMMAND)
{
    std::string str = "abrakadabra";
    commands_factory factory;
    auto cmd = factory.create_command(str);

    ASSERT_EQ(cmd->type(), command_type::text);
    ASSERT_EQ(dynamic_cast<text_command*>(cmd.get())->info(), str);
}

TEST(COMMAND_HANDLER, BASE_CHECK)
{
    auto subscriber = std::make_shared<subscriber_mock>();
    command_handler cmd_handler(1);
    cmd_handler.subscribe(subscriber);

    std::string command_text("hello_world");
    uint64_t timestamp = 123456;
    cmd_handler.add_command(std::make_unique<text_command>(timestamp, command_text));

    std::vector<std::string> expected;
    expected.push_back(command_text);
    ASSERT_TRUE(expected == subscriber->output());
    ASSERT_EQ(timestamp, subscriber->timestamp());
}

TEST(COMMAND_HANDLER, DATA_ENDED_IN_BASE_SCOPE)
{
    auto subscriber = std::make_shared<subscriber_mock>();
    command_handler cmd_handler(2);
    cmd_handler.subscribe(subscriber);


    std::string command_text("hello_world");
    uint64_t timestamp = 123456;
    cmd_handler.add_command(std::make_unique<text_command>(timestamp, command_text));
    cmd_handler.add_command(std::make_unique<finish_command>(11));

    std::vector<std::string> expected;
    expected.push_back(command_text);
    ASSERT_TRUE(expected == subscriber->output());
    ASSERT_EQ(timestamp, subscriber->timestamp());
}

TEST(COMMAND_HANDLER, FORCED_CLOSURE_BASE_SCOPE)
{
    auto subscriber = std::make_shared<subscriber_mock>();
    command_handler cmd_handler(2);
    cmd_handler.subscribe(subscriber);

    std::string command_text("hello_world");
    uint64_t timestamp = 123456;
    cmd_handler.add_command(std::make_unique<text_command>(timestamp, command_text));
    cmd_handler.add_command(std::make_unique<open_scope_command>(11));

    std::vector<std::string> expected;
    expected.push_back(command_text);
    ASSERT_TRUE(expected == subscriber->output());
    ASSERT_EQ(timestamp, subscriber->timestamp());
}

TEST(COMMAND_HANDLER, ONE_NESTED_SCOPE)
{
    auto subscriber = std::make_shared<subscriber_mock>();
    command_handler cmd_handler(5);
    cmd_handler.subscribe(subscriber);

    std::string command_text("hello_world");
    uint64_t timestamp = 123456;
    cmd_handler.add_command(std::make_unique<open_scope_command>(11));
    cmd_handler.add_command(std::make_unique<text_command>(timestamp, command_text));
    cmd_handler.add_command(std::make_unique<close_scope_command>(12));

    std::vector<std::string> expected;
    expected.push_back(command_text);
    ASSERT_TRUE(expected == subscriber->output());
    ASSERT_EQ(timestamp, subscriber->timestamp());
}


TEST(COMMAND_HANDLER, MULTIPLE_NESTED_SCOPES)
{
    auto subscriber = std::make_shared<subscriber_mock>();
    command_handler cmd_handler(5);
    cmd_handler.subscribe(subscriber);

    std::string command1_text("hello_world");
    std::string command2_text("hello_world");
    std::string command3_text("hello_world");
    uint64_t timestamp1 = 111111;
    uint64_t timestamp2 = 222222;
    uint64_t timestamp3 = 333333;

    cmd_handler.add_command(std::make_unique<open_scope_command>(1));
    cmd_handler.add_command(std::make_unique<text_command>(timestamp1, command1_text));

    cmd_handler.add_command(std::make_unique<open_scope_command>(2));
    cmd_handler.add_command(std::make_unique<text_command>(timestamp2, command2_text));
    cmd_handler.add_command(std::make_unique<close_scope_command>(3));

    cmd_handler.add_command(std::make_unique<text_command>(timestamp3, command3_text));
    cmd_handler.add_command(std::make_unique<close_scope_command>(4));

    std::vector<std::string> expected;
    expected.push_back(command1_text);
    expected.push_back(command2_text);
    expected.push_back(command3_text);
    ASSERT_TRUE(expected == subscriber->output());
    ASSERT_EQ(timestamp1, subscriber->timestamp());
}

TEST(COMMAND_HANDLER, FORCED_CLOSURE_NESTED_SCOPE)
{
    auto subscriber = std::make_shared<subscriber_mock>();
    command_handler cmd_handler(2);
    cmd_handler.subscribe(subscriber);

    std::string command_text("hello_world");
    uint64_t timestamp = 123456;
    cmd_handler.add_command(std::make_unique<open_scope_command>(11));
    cmd_handler.add_command(std::make_unique<text_command>(timestamp, command_text));
    cmd_handler.add_command(std::make_unique<finish_command>(12));

    std::vector<std::string> expected;
    ASSERT_TRUE(expected == subscriber->output());
    ASSERT_EQ(0, subscriber->timestamp());
}


