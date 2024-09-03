// Copyright 2024 Vladislav Aleinik
//==================================================
// Emulator Command Representation
//==================================================

#include <arch.hpp>
#include <command.hpp>

#include <cstdio>
#include <stdexcept>

//----------------------
// Command construction
//----------------------

CommandPush::CommandPush(Val_t val) :
    val_ (val)
{ }

CommandPushr::CommandPushr(Reg_t reg) :
    reg_ (reg)
{ }

CommandPopr::CommandPopr(Reg_t reg) :
    reg_ (reg)
{ }

//-------------------
// Command execution
//-------------------

void CommandBegin::execute() const
{
    printf("BEGIN\n");
}

void CommandEnd::execute() const
{
    printf("END\n");
}

void CommandPush::execute() const
{
    printf("PUSH %d\n", val_);
}

void CommandPop::execute() const
{
    printf("POP\n");
}

void CommandPushr::execute() const
{
    printf("PUSHR %s\n", get_register_name(reg_).c_str());
}

void CommandPopr::execute() const
{
    printf("POPR %s\n", get_register_name(reg_).c_str());
}

//--------------------------
// Polymorphic command type
//--------------------------

Command::Command() :
    type_ (CommandType::NOTHING),
    ptr_  (nullptr)
{ }

Command::Command(CommandBegin* cmd) :
    type_ (CommandType::BEGIN),
    ptr_  (cmd)
{ }

Command::Command(CommandEnd* cmd) :
    type_ (CommandType::END),
    ptr_  (cmd)
{ }

Command::Command(CommandPush* cmd) :
    type_ (CommandType::PUSH),
    ptr_  (cmd)
{ }

Command::Command(CommandPop* cmd) :
    type_ (CommandType::POP),
    ptr_  (cmd)
{ }

Command::Command(CommandPushr* cmd) :
    type_ (CommandType::PUSHR),
    ptr_  (cmd)
{ }

Command::Command(CommandPopr* cmd) :
    type_ (CommandType::POPR),
    ptr_  (cmd)
{ }


Command::Command(Command&& that) :
    type_ (that.type_),
    ptr_  (that.ptr_)
{
    that.type_ = CommandType::NOTHING;
    that.ptr_  = nullptr;
}

Command& Command::operator=(Command&& that)
{
    this->release();

    type_ = that.type_;
    ptr_  = that.ptr_;

    that.type_ = CommandType::NOTHING;
    that.ptr_  = nullptr;

    return *this;
}

void Command::execute() const
{
    // Dispatch function based on it's type.
    // NOTE: this is a boilerplate code a compiler
    //       is definitely able to produce for us.
    // NOTE: use virtual functions and dynamic_cast
    switch (type_)
    {
        case CommandType::BEGIN:
        {
            CommandBegin* ptr = static_cast<CommandBegin*>(ptr_);
            ptr->execute();
            break;
        }
        case CommandType::END:
        {
            CommandEnd* ptr = static_cast<CommandEnd*>(ptr_);
            ptr->execute();
            break;
        }
        case CommandType::PUSH:
        {
            CommandPush* ptr = static_cast<CommandPush*>(ptr_);
            ptr->execute();
            break;
        }
        case CommandType::POP:
        {
            CommandPop* ptr = static_cast<CommandPop*>(ptr_);
            ptr->execute();
            break;
        }
        case CommandType::PUSHR:
        {
            CommandPushr* ptr = static_cast<CommandPushr*>(ptr_);
            ptr->execute();
            break;
        }
        case CommandType::POPR:
        {
            CommandPopr* ptr = static_cast<CommandPopr*>(ptr_);
            ptr->execute();
            break;
        }
        default:
        {
            throw std::runtime_error("Command::execute(): invalid Command type");
        }
    }
}

void Command::release()
{
    if (type_ == CommandType::NOTHING)
    {
        return;
    }

    // Dispatch destructor based on it's type:
    // NOTE: oh sh*t here we go again.
    switch (type_)
    {
        case CommandType::BEGIN:
        {
            CommandBegin* ptr = static_cast<CommandBegin*>(ptr_);
            delete ptr;
            break;
        }
        case CommandType::END:
        {
            CommandEnd* ptr = static_cast<CommandEnd*>(ptr_);
            delete ptr;
            break;
        }
        case CommandType::PUSH:
        {
            CommandPush* ptr = static_cast<CommandPush*>(ptr_);
            delete ptr;
            break;
        }
        case CommandType::POP:
        {
            CommandPop* ptr = static_cast<CommandPop*>(ptr_);
            delete ptr;
            break;
        }
        case CommandType::PUSHR:
        {
            CommandPushr* ptr = static_cast<CommandPushr*>(ptr_);
            delete ptr;
            break;
        }
        case CommandType::POPR:
        {
            CommandPopr* ptr = static_cast<CommandPopr*>(ptr_);
            delete ptr;
            break;
        }
        default:
        {
            // NOTE: from C++11 destructors are ‘noexcept’ by default
            // throw std::runtime_error("Command::~Command(): invalid Command type");
        }
    }

    type_ = CommandType::NOTHING;
    ptr_ = nullptr;
}

Command::~Command()
{
    release();
}
