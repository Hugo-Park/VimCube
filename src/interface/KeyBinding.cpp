#include <iostream>
#include "KeyBinding.h"

/*
    Function Name : KeyBinding 
    Parameters : -
    Return Type : -
    Description : Constructor for KeyBinding Class
*/
KeyBinding::KeyBinding()
{ }

/*
    Function Name : KeyBinding 
    Parameters : std::map<std::string, Action> keyBindingMap
    Return Type : -
    Description : Constructor for KeyBinding Class
*/
KeyBinding::KeyBinding(std::map<std::string, Action> keyBindingMap)
    : keyBindingMap(keyBindingMap)
{ }

/*
    Function Name : inertKeyToMap
    Parameters : std::string str, Action action
    Return Type : void
    Description : Insert a key binding to keybinding map
*/
void KeyBinding::insertKeyToMap(std::string str, Action action)
{
    keyBindingMap[str] = action;
}

/*
    Function Name : runKeyBinding
    Parameters : std::string str
    Return Type : bool
    Description : Run input key binding 
*/
bool KeyBinding::runKeyBinding(ftxui::Event event)
{
    if (!event.is_character())
        return false;

    Dq.push_back(event.character()[0]);
    std::string str(Dq.begin(), Dq.end());  // make string with char in deque
    keyBuffer = str;

    auto it = keyBindingMap.find(keyBuffer);
    if (it != keyBindingMap.end())
    {
        it->second();
        this->keyBuffer.clear();
        this->Dq.clear();
        return true;
    }

    // if deque size is over 4 (if any string matches with keybindings)
    if (Dq.size() >= 4)
    {
        Dq.clear();
        Dq.push_back(event.character()[0]);
        keyBuffer.clear();
        std::string str(Dq.begin(), Dq.end());  // make string with char in deque
        keyBuffer = str;
    }

    return false;
}

/*
    Function Name : printKeyBuffer 
    Parameters : -
    Return Type : std::string
    Description : Return keyBuffer for printing on screen
*/
std::string KeyBinding::printKeyBuffer()
{
    return keyBuffer;
}

/*
    Function Name : setPreDefinedKeyBinding
    Parameters : VimCubeApp& vimCube
    Return Type : void
    Description : Set dictonary to predefined key binding. Edit this function to add new key binding(dev).
*/
void KeyBinding::setPreDefinedKeyBinding(VimCubeApp& vimCube, std::vector<std::string>& commandHistory)
{
    this->insertKeyToMap("qq", [&]() { commandHistory.clear(); vimCube.setActiveTab(0); });
}