#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include <vector>
#include <cstddef>
#include <string>
#include "VimCubeApp.h"
#include "../command/Command.h"

namespace vimcube::command_executor{

    struct ExecuteResult
    {
        bool ok = false;
        std::string errorMsg;
        size_t errorIndex = 0;

        ExecuteResult() = default;
        ExecuteResult(bool ok, std::string erMsg, size_t erIdx) : ok(ok), errorMsg(erMsg), errorIndex(erIdx) {}
        ExecuteResult(bool ok) : ok(ok) {}
    };

    ExecuteResult execute(interface::VimCubeApp& app, const std::vector<vimcube::command::Command>& cmd);
}
#endif
