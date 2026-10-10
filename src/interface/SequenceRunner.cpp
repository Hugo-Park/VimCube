#include "SequenceRunner.h"
#include <string>
#include "../command/CommandParser.h"
#include "CommandExecutor.h"

namespace vimcube::interface {

    RunnerResult runSequence(VimCubeApp& app, const std::string& input)
    {
        vimcube::command::ParseResult parseResult = vimcube::command::parse(input);
        if (!parseResult.ok)
        {
            std::string errMsg = "[Parse Error] " + parseResult.errorMsg + " at " + std::to_string(parseResult.errorPos);
            return RunnerResult(false, errMsg);
        }

        vimcube::command_executor::ExecuteResult exeResult = vimcube::command_executor::execute(app, parseResult.commands);
        if (!exeResult.ok)
        {
            std::string errMsg = "[Execute Error] " + exeResult.errorMsg + " at " + std::to_string(exeResult.errorIndex);
            return RunnerResult(false, errMsg);
        }

        return RunnerResult(true);
    }
}
