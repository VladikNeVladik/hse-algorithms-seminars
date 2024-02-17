// No copyright. Vladislav Aleinik, 2024
//===================================================
// Recursive Descent Parser for the regular language
//===================================================

#include <arch.hpp>
#include <parser.hpp>
#include <utils.hpp>

#include <stdexcept>
#include <cstring>

//----------------------
// Existance management
//----------------------

Parser::Parser(const char* filename) :
    file_ (std::ifstream(filename, std::ios::in)),
    pos_  (/* default constructor */),
    end_  (/* default constructor */)
{
    VERIFY_CONTRACT(file_.good(),
        "Unable to open file \'%s\'\n",
        filename);

    // Initialize the first line:
    read_line_from_file();
}

//------------------
// Helper utilities
//------------------

void Parser::read_line_from_file()
{
    file_.getline(line_, MAX_LINE_SIZE);

    VERIFY_CONTRACT(
        file_.good() || file_.eof(),
        "Unable to read input line\n");

    pos_ = line_;
    end_ = line_ + std::strlen(line_);
}

bool Parser::parse_pattern(std::regex regexp)
{
    std::cmatch match_result{};
    bool match_status = std::regex_search(
        pos_, end_,                              // The iterators limiting the matched sequence
        match_result,                            // Match result
        regexp,                                  // Regular expression specifying the pattern
        std::regex_constants::match_continuous); // Start matching from the beginning

    // Move the iterator on success:
    if (match_status)
    {
        pos_ = match_result[0].second;
    }

    return match_status;
}

bool Parser::parse_pattern(std::regex regexp, std::string& ret)
{
    std::cmatch match_result{};
    bool match_status = std::regex_search(
        pos_, end_,                              // The iterators limiting the matched sequence
        match_result,                            // Match result
        regexp,                                  // Regular expression specifying the pattern
        std::regex_constants::match_continuous); // Start matching from the beginning

    // Move the iterator on success:
    if (match_status)
    {
        pos_ = match_result[0].second;

        ret = std::string(match_result[0].first, match_result[0].second);
    }

    return match_status;
}

//--------------------------
// Recursive Descent Parser
//--------------------------

bool Parser::parse_space_sequence()
{
    static const std::regex pattern{"[ \t]+"};

    return parse_pattern(pattern);
}

bool Parser::parse_newline_sequence()
{
    bool success = (pos_ == end_);

    while (pos_ == end_ && !file_.eof())
    {
        read_line_from_file();
    }

    return success;
}

bool Parser::parse_end_of_file()
{
    return file_.eof();
}

Cmd_t Parser::parse_command_name()
{
    static const std::regex pattern{"[a-zA-Z]+"};

    // Skip leading whitespaces (may be none):
    parse_space_sequence();

    // Perform parsing:
    std::string cmd_name;
    bool success = parse_pattern(pattern, cmd_name);
    if (!success)
    {
        throw std::runtime_error("Unable to parse command name!\n");
    }

    // Throws an exception:
    return get_command_id(cmd_name);
}

Reg_t Parser::parse_register_name()
{
    static const std::regex pattern{"[a-zA-Z]+"};

    // Skip leading whitespaces:
    bool success = parse_space_sequence();
    if (!success)
    {
        throw std::runtime_error("Expected a space sequence before command name!\n");
    }

    // Perform parsing:
    std::string reg_name;
    success = parse_pattern(pattern, reg_name);
    if (!success)
    {
        throw std::runtime_error("Expected a register name!\n");
    }

    // Throws an exception:
    return get_register_id(reg_name);
}

Val_t Parser::parse_integral_value()
{
    static const std::regex pattern{"(\\+|-)?(0|[1-9][0-9]*)"};

    // Skip leading whitespaces:
    bool success = parse_space_sequence();
    if (!success)
    {
        throw std::runtime_error("Expected a space sequence before integral value!\n");
    }

    // Perform parsing:
    std::string val_str;
    success = parse_pattern(pattern, val_str);
    if (!success)
    {
        throw std::runtime_error("Expected an integral value!\n");
    }

    return get_value(val_str);
}

Command Parser::parse_command_line()
{
    // Parse command name:
    Cmd_t cmd_id = parse_command_name();

    // Dispatch based on command format:
    Command to_return{};
    switch (cmd_id)
    {
        case CommandType::BEGIN:
        {
            to_return = Command(new CommandBegin());
            break;
        }
        case CommandType::END:
        {
            to_return = Command(new CommandEnd());
            break;
        }
        case CommandType::PUSH:
        {
            Val_t val = parse_integral_value();

            to_return = Command(new CommandPush(val));
            break;
        }
        case CommandType::POP:
        {
            to_return = Command(new CommandPop());
            break;
        }
        case CommandType::PUSHR:
        {
            Reg_t reg = parse_register_name();

            to_return = Command(new CommandPushr(reg));
            break;
        }
        case CommandType::POPR:
        {
            Reg_t reg = parse_register_name();

            to_return = Command(new CommandPopr(reg));
            break;
        }
        default:
        {
            throw std::runtime_error("Parser::parse_command_line(): invalid command id");
        }
    }

    // Parse newline:
    // NOTE: as in C, every program is expected to end with newline.
    // NOTE: in C it is Undefined Behavior to skip trailing file newline.
    parse_newline_sequence();

    return to_return;
}

std::vector<Command> Parser::parse_command_sequence()
{
    std::vector<Command> commands{};

    while (!parse_end_of_file())
    {
        commands.push_back(parse_command_line());
    }

    return commands;
}
