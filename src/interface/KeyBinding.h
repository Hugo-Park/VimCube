#ifndef KEY_BINDING_H
#define KEY_BINDING_H
#include <map>
#include <deque>
#include <string>
#include <functional>
#include <ftxui/component/event.hpp>
#include "VimCubeApp.h"

namespace vimcube::interface {
    class KeyBinding
    {
    using Action = std::function<void()>;
    private:
        std::map<std::string, Action> keyBindingMap;
        std::string keyBuffer;
        std::deque<char> Dq;

    public:
        KeyBinding();
        KeyBinding(std::map<std::string, Action> keyBindingMap);
        void insertKeyToMap(std::string, Action action);
        bool runKeyBinding(ftxui::Event event);
        std::string printKeyBuffer();
        void setPreDefinedKeyBinding(VimCubeApp& vimCube, std::vector<std::string>& commandHistory);
    };
}
#endif