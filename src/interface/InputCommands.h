#ifndef INPUT_COMMANDS_H
#define INPUT_COMMANDS_H
#include <map>
#include <string>
#include <functional>
#include "VimCubeApp.h"

namespace vimcube::interface {
    class InputCommands
    {
    using Action = std::function<void(const std::vector<std::string>& args)>;
    private:
        std::map<std::string, Action> inputCommandMap;

    public:
        InputCommands();
        InputCommands(std::map<std::string, Action> input);
        void insertItemToMap(std::string str, Action action);
        bool runCommand(std::string str);
        void setPredefinedCommands(VimCubeApp& vimCube, std::vector<std::string>& commandHistory);
    };
}
#endif