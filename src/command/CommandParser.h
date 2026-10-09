#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <vector>
#include <string>
#include "Command.h"
namespace vimcube::command {

    struct ParseResult
    {
        bool ok = false;
        std::vector<vimcube::command::Command> commands;    // Parsed commands
        std::string errorMsg;                               // Error message
        size_t errorPos = 0;                                // Position of error string
    };

    ParseResult parse(const std::string& input);
}

#endif
