#ifndef SEQUENCE_RUNNER_H
#define SEQUENCE_RUNNER_H

#include <string>
#include "VimCubeApp.h"

namespace vimcube::interface {

    struct RunnerResult
    {
        bool ok = false;
        std::string errorMsg;

        RunnerResult() = default;
        RunnerResult(bool ok) : ok(ok) {}
        RunnerResult(bool ok , std::string errMsg) : ok(ok), errorMsg(errMsg) {}
    };

    RunnerResult runSequence(VimCubeApp& app, const std::string& input);
}

#endif
