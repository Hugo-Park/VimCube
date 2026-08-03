#ifndef VIMCUBE_APP_H
#define VIMCUBE_APP_H
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/terminal.hpp>

class VimCubeApp
{
private:
    int activeTab = 0;

public:
    VimCubeApp();
    void setActiveTab(int tabNum);
    ftxui::Component createTitleScreen(ftxui::ScreenInteractive& screen);
    ftxui::Component createWorkSpace(ftxui::ScreenInteractive& screen);
    ftxui::Component createTab(ftxui::ScreenInteractive& screen);
};

#endif