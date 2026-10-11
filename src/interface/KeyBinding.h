#ifndef KEY_BINDING_H
#define KEY_BINDING_H
#include <map>
#include <string>
#include <functional>
#include <ftxui/component/event.hpp>
#include "SequenceRunner.h"
#include "VimCubeApp.h"

namespace vimcube::interface {

    enum class Mode { NORMAL, SEQUENCE };

    class KeyBinding
    {
    using Action = std::function<void()>;
    private:
        std::map<std::string, Action> keyBindingMap;

        Mode mode = Mode::NORMAL;
        std::string sequenceBuffer;
        VimCubeApp* app = nullptr;

        std::string statusMessage;
        bool incomplete = false;

        void updatePreview();
        bool checkCharacter(const char c);

    public:
        KeyBinding();
        KeyBinding(std::map<std::string, Action> keyBindingMap);
        void insertKeyToMap(std::string, Action action);
        bool runKeyBinding(ftxui::Event event);
        std::string printKeyBuffer();
        void setPreDefinedKeyBinding(VimCubeApp& vimCube);

        const std::string& getStatusMessage() const;

        bool isSequence() const;
    };
}
#endif
