#ifndef VIMCUBE_APP_H
#define VIMCUBE_APP_H
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/terminal.hpp>
#include "../render/GeoDraw.h"

namespace vimcube::interface {
    class VimCubeApp
    {
    using Geometry = std::variant<vimcube::geometry::Mesh>;
    private:
        int activeTab = 0;
        std::vector<Geometry> sceneGeos;

    public:
        VimCubeApp();
        void setActiveTab(int tabNum);
        ftxui::Component createTitleScreen(ftxui::ScreenInteractive& screen);
        ftxui::Component createWorkSpace(ftxui::ScreenInteractive& screen);
        ftxui::Component createTab(ftxui::ScreenInteractive& screen);
        void addGeoToScene(const Geometry& geometry);
    };
}
#endif