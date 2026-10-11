#include "KeyBinding.h"
#include <ftxui/component/event.hpp>
#include "../command/CommandParser.h"
#include "CommandExecutor.h"
#include "SequenceRunner.h"

namespace vimcube::interface {
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
        // In sequence mode
        if (this->mode == Mode::SEQUENCE)
        {
            if (event == ftxui::Event::Escape)
            {
                app->restoreSnapshot();
                app->discardSnapshot();
                this->sequenceBuffer.clear();
                this->mode = Mode::NORMAL;
            }

            else if (event == ftxui::Event::Character(';'))
            {
                app->discardSnapshot();
                app->clearSelectGeo();
                this->sequenceBuffer.clear();
                this->mode = Mode::NORMAL;
            }

            return true;
        }

        // In normal mode
        else if (this->mode == Mode::NORMAL)
        {
            if (!event.is_character())
                return false;
            std::string c = event.character();
            vimcube::command::VerbType type = vimcube::command::classifyVerbStart(c[0]);

            if (type == vimcube::command::VerbType::CREATE)
            {
                if (app->getSelectedGeo() == nullptr)
                {
                    app->saveSnapshot();
                    this->sequenceBuffer.append(c);
                    this->mode = Mode::SEQUENCE;
                    return true;
                }
            }

            else if (type == vimcube::command::VerbType::EDIT)
            {
                if (app->getSelectedGeo() != nullptr)
                {
                    app->saveSnapshot();
                    this->sequenceBuffer.append(c);
                    this->mode = Mode::SEQUENCE;
                    return true;
                }
            }

            else if (type == vimcube::command::VerbType::NONE)
            {
                auto it = keyBindingMap.find(c);

                // If find successfully
                if (it != keyBindingMap.end())
                {
                    it->second();
                    return true;
                }
                return false;
            }
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
        return this->sequenceBuffer;
    }

    /*
        Function Name : setPreDefinedKeyBinding
        Parameters : VimCubeApp& vimCube
        Return Type : void
        Description : Set dictonary to predefined key binding. Edit this function to add new key binding(dev).
    */
    void KeyBinding::setPreDefinedKeyBinding(VimCubeApp& vimCube)
    {
        this->app = &vimCube;

        this->insertKeyToMap("n", [&]() { vimCube.selectNextGeo(); });
        this->insertKeyToMap("N", [&]() { vimCube.selectPrevGeo(); });

        this->insertKeyToMap("H", [&]() { vimCube.getCamera().orbitLeft(0.2f); });
        this->insertKeyToMap("L", [&]() { vimCube.getCamera().orbitRight(0.2f); });
        this->insertKeyToMap("J", [&]() { vimCube.getCamera().orbitDown(0.2f); });
        this->insertKeyToMap("K", [&]() { vimCube.getCamera().orbitUp(0.2f); });

        this->insertKeyToMap("h", [&]() { vimCube.getCamera().orbitLeft(0.1f); });
        this->insertKeyToMap("l", [&]() { vimCube.getCamera().orbitRight(0.1f); });
        this->insertKeyToMap("j", [&]() { vimCube.getCamera().orbitDown(0.1f); });
        this->insertKeyToMap("k", [&]() { vimCube.getCamera().orbitUp(0.1f); });

        this->insertKeyToMap("A", [&]() { vimCube.getCamera().zoomIn(0.01f); });
        this->insertKeyToMap("S", [&]() { vimCube.getCamera().zoomOut(0.01f); });

        this->insertKeyToMap("a", [&]() { vimCube.getCamera().zoomIn(0.005f); });
        this->insertKeyToMap("s", [&]() { vimCube.getCamera().zoomOut(0.005f); });
    }

    bool KeyBinding::isSequence() const
    {
        return (this->mode == Mode::SEQUENCE);
    }
}
