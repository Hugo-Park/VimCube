#include <iostream>
#include "InputCommands.h"

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
    auto it = inputCommandMap.find(str);
    if (it != inputCommandMap.end())
    {
        it->second();
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
    this->insertItemToMap("q", [&]() { commandHistory.clear(); vimCube.setActiveTab(0); });
    this->insertItemToMap("clear", [&]() { commandHistory.clear(); });
}