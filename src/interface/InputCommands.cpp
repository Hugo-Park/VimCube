#include "InputCommands.h"
#include "../geometry/GeoFactory.h"
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
        this->insertItemToMap("select", [&](const std::vector<std::string>& args) { 

            uint32_t id = 0;

            if (!args.empty()) {
                try {
                    id = std::stoi(args[0]);
                }

                catch (const std::invalid_argument& e) {
                    return;
                }
            }
            vimCube.selectGeo(id);
        });

        this->insertItemToMap("deselect", [&](const std::vector<std::string>& args) { 

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

        this->insertItemToMap("delete", [&](const std::vector<std::string>& args) { 

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

        this->insertItemToMap("cube", [&](const std::vector<std::string>& args) {
            float size = 10.0f;

            if (!args.empty()) {
                try {
                    size = std::stof(args[0]);
                }

                catch (const std::invalid_argument& e) {
                    return;
                }
            }

            vimcube::geometry::Mesh returnMesh = vimcube::geo_factory::createCube(size);
            vimCube.addGeoToScene(returnMesh);
        });

        this->insertItemToMap("sphere", [&](const std::vector<std::string>& args) {
            float radius = 10.0f;
            uint32_t rings = 10;
            uint32_t segments = 10;

            if (!args.empty()){
                try {
                    radius = std::stof(args[0]);
                    rings = std::stof(args[1]);
                    segments = std::stof(args[2]);
                }

                catch (const std::invalid_argument& e) {
                    return;
                }
            }

            vimcube::geometry::Mesh returnMesh = vimcube::geo_factory::createSphere(radius, rings, segments);
            vimCube.addGeoToScene(returnMesh);
        });
    }
}
