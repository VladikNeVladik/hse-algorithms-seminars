// No copyright. Vladislav Aleinik, 2024
//===============================================
// Specification of the virtual CPU architecture
//===============================================

#include <arch.hpp>
#include <command.hpp>

#include <vector>
#include <stdexcept>

//---------------
// CPU commands
//---------------

static std::vector<std::string> CMD_NAME_TO_CMD_ID = {
    "NOTHING", // CommandType::NOTHING
    "BEGIN",   // CommandType::BEGIN
    "END",     // CommandType::END
    "PUSH",    // CommandType::PUSH
    "POP",     // CommandType::POP
    "PUSHR",   // CommandType::PUSHR
    "POPR"     // CommandType::POPR
};

Cmd_t get_command_id(std::string str)
{
    for (size_t id = 0U; id < CMD_NAME_TO_CMD_ID.size(); ++id)
    {
        if (CMD_NAME_TO_CMD_ID[id] == str)
        {
            return id;
        }
    }

    throw std::runtime_error("parse_command_name(): unknown command name!");
}

//---------------
// CPU registers
//---------------

static std::vector<std::string> REG_NAME_TO_REG_ID = {
    "AX",
    "BX",
    "CX",
    "DX",
    "EX",
    "FX",
    "PC"
};

Reg_t get_register_id(std::string str)
{
    for (size_t id = 0U; id < REG_NAME_TO_REG_ID.size(); ++id)
    {
        if (REG_NAME_TO_REG_ID[id] == str)
        {
            return id;
        }
    }

    throw std::runtime_error("parse_register_name(): unknown register name!");
}

std::string get_register_name(Reg_t reg)
{
    return REG_NAME_TO_REG_ID.at(reg);
}

//------------
// CPU Values
//------------

Val_t get_value(std::string str)
{
    // NOTE: If no conversion can be performed, 0 is returned.
    return std::atoi(str.c_str());
}
