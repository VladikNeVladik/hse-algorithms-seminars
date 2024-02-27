// No copyright. Vladislav Aleinik, 2024
//==================================================
// Emulator Command Representation
//==================================================
#ifndef HEADER_GUARD_COMMAND_HPP_INCLUDED
#define HEADER_GUARD_COMMAND_HPP_INCLUDED

#include <arch.hpp>

//------------------
// Command subtypes
//------------------

enum CommandType : Cmd_t
{
    NOTHING = 0,
    BEGIN   = 1,
    END     = 2,
    PUSH    = 3,
    POP     = 4,
    PUSHR   = 5,
    POPR    = 6
};

class CommandBegin
{
public:
    CommandBegin() = default;

    void execute() const;
};

class CommandEnd
{
public:
    CommandEnd() = default;

    void execute() const;
};

class CommandPush
{
public:
    CommandPush(Val_t val);

    void execute() const;
private:
    Val_t val_;
};

class CommandPop
{
public:
    CommandPop() = default;

    void execute() const;
};

class CommandPushr
{
public:
    CommandPushr(Reg_t reg);

    void execute() const;
private:
    Reg_t reg_;
};

class CommandPopr
{
public:
    CommandPopr(Reg_t reg);

    void execute() const;
private:
    Reg_t reg_;
};

//--------------------------
// Polymorphic command type
//--------------------------

class Command
{
public:
    // Existance management:
    Command();
    Command(CommandBegin* cmd);
    Command(CommandEnd*   cmd);
    Command(CommandPush*  cmd);
    Command(CommandPop*   cmd);
    Command(CommandPushr* cmd);
    Command(CommandPopr*  cmd);

    ~Command();

    // Ensure command is never copied:
    Command(const Command&) = delete;
    Command(Command&&);

    Command& operator=(const Command&) = delete;
    Command& operator=(Command&&);

    // Command execution:
    void execute() const;

private:
    // Memory management utility:
    void release();

    CommandType type_;

    void* ptr_;
};

#endif // HEADER_GUARD_COMMAND_HPP_INCLUDED
