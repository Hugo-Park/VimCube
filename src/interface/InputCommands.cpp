#include "InputCommands.h"
#include "VimCubeApp.h"
#include <cstddef>
#include <cstdint>
#include <vector>

namespace vimcube::interface {
    /*
        Function Name : InputCommands
        Parameters : -
        Return Type : -
        Description : Constructor for InputCommands Class
    */
    InputCommands::InputCommands()
    { }

    /*
        Function Name : InputCommands
        Parameters : std::map<int, std::string> input
        Return Type : -
        Description : Constructor for InputCommands Class
    */
    InputCommands::InputCommands(std::map<std::string, Action> input)
        : inputCommandMap(input)
    { }

    /*
        Function Name : insertItemToMap
        Parameters : std::string str, Action action
        Return Type : void
        Description : Insert a command to command map
    */
    void InputCommands::insertItemToMap(std::string str, Action action)
    {
        inputCommandMap[':' + str] = action;
    }

    /*
        Function Name : runCommand
        Parameters : std::string str
        Return Type : bool
        Description : Run input command
    */
    bool InputCommands::runCommand(std::string str)
    {
        std::stringstream ss(str);
        std::string command;
        std::vector<std::string> args;

        ss >> command;
        this->commandBuffer = command;

        std::string token;
        while (ss >> token) {
            args.push_back(token);
        }

        auto it = inputCommandMap.find(command);
        if (it != inputCommandMap.end())
        {
            it->second(args);
            return true;
        }
        return false;
    }

    std::string InputCommands::printCommandBuffer()
    {
        return this->commandBuffer;
    }

    /*
        Function Name : setPredefinedCommands
        Parameters : VimCubeApp& vimCube, std::vector<std::string>& commandHistory
        Return Type : void
        Description : Set dictonary to predefined commands. Edit this function to add new commands(dev).
    */
    void InputCommands::setPredefinedCommands(VimCubeApp& vimCube)
    {
        /*
            Syntax: q
            Description : Return to title screen
        */
        this->insertItemToMap("q", [&](const std::vector<std::string>& args) { vimCube.setActiveTab(0); vimCube.resetApp(); });

        /*
            Syntax: s <id>
            Description : Select geometry by id
        */
        this->insertItemToMap("s", [&](const std::vector<std::string>& args) { 

            uint32_t id = 0;

            if (!args.empty()) {
                try {
                    id = std::stoi(args[0]);
                }

                catch (const std::invalid_argument& e) {
                    return;
                }
            }
            vimCube.selectGeo(id, true);
        });

        /*
            Syntax: ds <id>
            Description : Deselect geometry by id
        */
        this->insertItemToMap("ds", [&](const std::vector<std::string>& args) { 

            uint32_t id = 0;

            if (!args.empty()) {
                try {
                    id = std::stoi(args[0]);
                }

                catch (const std::invalid_argument& e) {
                    return;
                }
            }
            vimCube.deselectGeo(id);
        });

        /*
            Syntax: ds
            Description : Deselect selected geometry
        */
        this->insertItemToMap("ds", [&](const std::vector<std::string>& args) { 

            vimCube.deselectGeo();
        });

        /*
            Syntax: d <id>
            Description : Delete geometry by id
        */
        this->insertItemToMap("d", [&](const std::vector<std::string>& args) { 

            uint32_t id = 0;

            if (!args.empty()) {
                try {
                    id = std::stoi(args[0]);
                }

                catch (const std::invalid_argument& e) {
                    return;
                }
            }
            vimCube.removeGeoFromScene(id);
        });

        /*
            Syntax: d
            Description : Delete selected geometry
        */
        this->insertItemToMap("d", [&](const std::vector<std::string>& args) { 

            if (vimCube.getSelectedGeo() != nullptr)
            {
                vimCube.removeGeoFromScene(vimCube.getSelectedGeo()->getId());
            }
        });
        
        /*
            Syntax: dall
            Description : Delete all geometry
        */
        this->insertItemToMap("dall", [&](const std::vector<std::string>& args) { 

            vimCube.removeAllGeoFromScene();
        });

        /*
            Syntax: gr
            Description : Toggle grid indicator
        */
        this->insertItemToMap("gr", [&](const std::vector<std::string>& args) {

            if (vimCube.getHasGrid()) vimCube.setHasGrid(false);
            else vimCube.setHasGrid(true);
        });

        /*
            Syntax: ax
            Description : Toggle axis indicator
        */
        this->insertItemToMap("ax", [&](const std::vector<std::string>& args) {

            if (vimCube.getHasAxisIndicator()) vimCube.setHasAxisIndicator(false);
            else vimCube.setHasAxisIndicator(true);
        });

        /*
            Syntax: f
            Description : Toggle focus mode
        */
        this->insertItemToMap("f", [&](const std::vector<std::string>& args) {

            if (vimCube.getIsFocusMode()) vimCube.setFocusMode(false);
            else vimCube.setFocusMode(true);
        });

        /*
            Syntax: o
            Description : Reset camera position
        */
        this->insertItemToMap("o", [&](const std::vector<std::string>& args) {

            vimCube.getCamera().resetCamera();
        });
    }
}
