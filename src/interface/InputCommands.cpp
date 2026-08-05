#include <iostream>
#include "InputCommands.h"
#include "../geometry/GeoFactory.h"

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

    /*
        Function Name : setPredefinedCommands
        Parameters : VimCubeApp& vimCube, std::vector<std::string>& commandHistory
        Return Type : void
        Description : Set dictonary to predefined commands. Edit this function to add new commands(dev).
    */
    void InputCommands::setPredefinedCommands(VimCubeApp& vimCube, std::vector<std::string>& commandHistory)
    {
        this->insertItemToMap("q", [&](const std::vector<std::string>& args) { commandHistory.clear(); vimCube.setActiveTab(0); });
        this->insertItemToMap("clear", [&](const std::vector<std::string>& args) { commandHistory.clear(); });
        this->insertItemToMap("cube", [&](const std::vector<std::string>& args) {
            float size = 10.0f;

            if (!args.empty()){
                try {
                    size = std::stof(args[0]);
                }

                catch (const std::invalid_argument& e) {
                    return;
                }
            }

            vimcube::geometry::Mesh resultMesh = vimcube::geo_factory::createCube(size);

            vimCube.addGeoToScene(resultMesh);
        });
    }
}