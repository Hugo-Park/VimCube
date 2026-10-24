#ifndef INPUT_COMMANDS_H
#define INPUT_COMMANDS_H
#include <map>
#include <string>
#include <functional>

namespace vimcube::interface {
    class VimCubeApp;

    class InputCommands
    {
    using Action = std::function<void(const std::vector<std::string>& args)>;
    private:
        std::map<std::string, Action> inputCommandMap;
        std::string commandBuffer;

    public:
        InputCommands();
        InputCommands(std::map<std::string, Action> input);
        void insertItemToMap(std::string str, Action action);
        bool runCommand(std::string str);
        std::string printCommandBuffer();
        void setPredefinedCommands(VimCubeApp& vimCube);

        void reset();
    };
}
#endif
