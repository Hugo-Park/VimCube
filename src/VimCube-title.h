#ifndef VIMCUBE_TITLE_H
#define VIMCUBE_TITLE_H
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/terminal.hpp>

/*
    Function Name : createTitleScreen
    Parameters : int* activeTab, ftxui::ScreenInteractive& screen
    Return Type : ftxui::Component
    Description : Create title screen by rendering title(left) and menu(right)
*/
ftxui::Component createTitleScreen(int* activeTab, ftxui::ScreenInteractive& screen);

/*
    Function Name : createWorkSpace
    Parameters : int* activeTab, ftxui::ScreenInteractive& screen
    Return Type : ftxui::Component
    Description : Create new workspace by rendering viewport(left) and command window(right)
*/
ftxui::Component createWorkSpace(int* activeTab, ftxui::ScreenInteractive& screen);

#endif