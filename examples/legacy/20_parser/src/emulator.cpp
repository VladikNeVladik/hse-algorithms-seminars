// Copyright 2024 Vladislav Aleinik
//=======================================
// Emulator for Made-Up CPU Architecture
//=======================================

#include <parser.hpp>

#include <cstdlib>
#include <cstdio>

int main(int argc, char* argv[])
{
    // Check the number of command line arguments:
    if (argc != 2)
    {
        printf("Usage: emulator <input file>\n");
        return EXIT_FAILURE;
    }

    // Get filename from command line:
    const char* filename = argv[1];

    // Perform parsing:
    Parser parser{filename};

    std::vector<Command> commands = parser.parse_command_sequence();

    // Execute all of the commands:
    // Note: use the range-for syntax:
    for (const auto& cmd : commands)
    {
        cmd.execute();
    }

    return EXIT_SUCCESS;
}
