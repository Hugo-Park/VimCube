#include "KeyBinding.h"
#include <cctype>
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
        Parameters : ftxui::Event event
        Return Type : bool
        Description : Run key-binding input
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
                this->statusMessage.clear();
                this->incomplete = false;
            }

            else if (event == ftxui::Event::Character(';'))
            {
                app->discardSnapshot();
                app->clearSelectGeo();
                this->sequenceBuffer.clear();
                this->mode = Mode::NORMAL;
                this->statusMessage.clear();
                this->incomplete = false;
            }

            else if (event.is_character())
            {
                if (checkCharacter(event.character()[0]))
                {
                    this->sequenceBuffer.append(event.character());
                    updatePreview();
                }
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
                    updatePreview();
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
                    updatePreview();
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

        // Select next/previous
        this->insertKeyToMap("n", [&]() { vimCube.selectNextGeo(); });
        this->insertKeyToMap("N", [&]() { vimCube.selectPrevGeo(); });

        // Rotate camera fast
        this->insertKeyToMap("H", [&]() { vimCube.getCamera().orbitLeft(0.2f); });
        this->insertKeyToMap("L", [&]() { vimCube.getCamera().orbitRight(0.2f); });
        this->insertKeyToMap("J", [&]() { vimCube.getCamera().orbitDown(0.2f); });
        this->insertKeyToMap("K", [&]() { vimCube.getCamera().orbitUp(0.2f); });

        // Rotate camrea normal
        this->insertKeyToMap("h", [&]() { vimCube.getCamera().orbitLeft(0.1f); });
        this->insertKeyToMap("l", [&]() { vimCube.getCamera().orbitRight(0.1f); });
        this->insertKeyToMap("j", [&]() { vimCube.getCamera().orbitDown(0.1f); });
        this->insertKeyToMap("k", [&]() { vimCube.getCamera().orbitUp(0.1f); });

        // Zoom in and out fast
        this->insertKeyToMap("A", [&]() { vimCube.getCamera().zoomIn(0.01f); });
        this->insertKeyToMap("S", [&]() { vimCube.getCamera().zoomOut(0.01f); });

        // Zoom in and out normal
        this->insertKeyToMap("a", [&]() { vimCube.getCamera().zoomIn(0.005f); });
        this->insertKeyToMap("s", [&]() { vimCube.getCamera().zoomOut(0.005f); });
    }

    /*
        Function Name : getStatusMessage 
        Parameters : -
        Return Type : const std::string&
        Description : Return statusMessage
    */
    const std::string& KeyBinding::getStatusMessage() const
    {
        return this->statusMessage;
    }

    /*
        Function Name : getMode 
        Parameters : -
        Return Type : Mode
        Description : Return mode
    */
    Mode KeyBinding::getMode() const
    {
        return this->mode;
    }

    /*
        Function Name : isSequence 
        Parameters : -
        Return Type : bool
        Description : Check isSequence
    */
    bool KeyBinding::isSequence() const
    {
        return (this->mode == Mode::SEQUENCE);
    }

    /*
        Function Name : isNormal
        Parameters : -
        Return Type : bool
        Description : Check isNormal
    */
    bool KeyBinding::isNormal() const
    {
        return (this->mode == Mode::NORMAL);
    }

    /*
        Function Name : updatePreview 
        Parameters : -
        Return Type : void
        Description : Run previewSequence() and update status
    */
    void KeyBinding::updatePreview()
    {
        vimcube::interface::RunnerResult r = vimcube::interface::previewSequence(*app, this->sequenceBuffer);
        this->statusMessage = r.errorMsg;
        this->incomplete = r.incomplete;
    }

    /*
        Function Name : checkCharacter 
        Parameters : const char c
        Return Type : bool
        Description : Check if character is valid
    */
    bool KeyBinding::checkCharacter(const char c)
    {
        return (vimcube::command::classifyVerbStart(c) != vimcube::command::VerbType::NONE || c == '+' || c == '-' || c == '.' || c == 'x' || c == 'y' || c == 'z' || ('0' <= c && c <= '9'));
    }
}
