#include "VimCube-title.h"
#include <iostream>
using namespace std;

/*
    Function Name : createTitleScreen
    Parameters : int* activeTab, ftxui::ScreenInteractive& screen
    Return Type : ftxui::Component
    Description : Create title screen by rendering menu(left) and title(right)
*/
ftxui::Component createTitleScreen(int* activeTab, ftxui::ScreenInteractive& screen)
{
    using namespace ftxui;

    static std::vector<std::string> entries {
        "New File",
        "Open File",
        "Quit(q)"
    };
    static int selected = 0;

    // Create menu and option
    MenuOption option;
    option.on_enter = [&screen, activeTab] {
        if (selected == 0)
        {
            *activeTab = 1;
            return;
        }

        else if (selected == 1)
        {
            *activeTab = 1;
            return;
        }

        else if (selected == 2)
        {
            screen.Exit();
            return;
        }
    };

    auto rightMenu = Menu(&entries, &selected, option);

    // Render left title window
    auto leftComponent = Renderer([&] {
        return vbox ({
            text("VimCube") | bold | center,
            separator(),
            text("Fast & Minimal Tool for Modeling") | dim | center,
        }) | borderEmpty | center | flex;
    });

    // Render all layouts
    auto finalLayout = Renderer(rightMenu, [=] {
        int width = ftxui::Terminal::Size().dimx;

        int leftWidth = width * 0.6;
        int rightWidth = width * 0.4;
        return hbox ({
            leftComponent->Render() | border | size(WIDTH, EQUAL, leftWidth) | flex,
            rightMenu->Render() | border | size(WIDTH, EQUAL, rightWidth) | flex,
        }) | flex;
    });

    // Keyboard event handling
    auto componentWithEvents = CatchEvent(finalLayout, [&](Event event) {
        if (event == Event::Character('q') || event == Event::Escape) {
            screen.Exit();
            return true;
        }
        return false;
    });

    return componentWithEvents;
}

/*
    Function Name : createWorkSpace
    Parameters : int* activeTab, ftxui::ScreenInteractive& screen
    Return Type : ftxui::Component
    Description : Create new workspace by rendering viewport(left) and command window(right)
*/
ftxui::Component createWorkSpace(int* activeTab, ftxui::ScreenInteractive& screen)
{
    using namespace ftxui;

    static std::string commandInput = "";
    static std::vector<std::string> commandHistory;

    // Create input box
    auto inputBox = Input(&commandInput, "");

    auto rightShell = Renderer(inputBox, [=] {
        Elements historyElements;

        if (commandHistory.empty()) {
            historyElements.push_back(text("Command Log Here...") | dim);
        }

        // Log auto-scrolling
        int maxLogs = ftxui::Terminal::Size().dimy - 6;
        if (maxLogs < 1) maxLogs = 1;

        int startIdx = 0;
        if (commandHistory.size() > maxLogs) {
            startIdx = commandHistory.size() - maxLogs;
        }

        for (size_t i = startIdx; i < commandHistory.size(); ++i) {
            historyElements.push_back(text(commandHistory[i]));
        }

        historyElements.push_back(filler());

        return vbox({
            text("Command Window") | bold | center,
            separator(),
            vbox(move(historyElements)) | flex,
            separator(),
            hbox({
                text("VimCube$") | bold,
                inputBox->Render()
            })
        }) | border;
    });

    // Render right canvas
    auto leftViewport = Renderer([=] {
        auto c = ftxui::Canvas(100, 100);
        
        c.DrawPointLine(10, 10, 90, 90, ftxui::Color::Red);

        return canvas(std::move(c)) | center | border;
    });

    // Render all layouts
    auto mainLayout = Renderer(rightShell, [=] {
        int width = ftxui::Terminal::Size().dimx;

        int leftWidth = width * 0.6;
        int rightWidth = width * 0.4;

        return hbox({
            leftViewport->Render() | size(WIDTH, EQUAL, leftWidth) | flex,
            rightShell->Render() | size(WIDTH, EQUAL, rightWidth) | flex
        }) | flex;
    });

    // Predefined commmand handling
    return CatchEvent(mainLayout, [=, &screen](Event event) {
        
        if (event == Event::Return) {
            if (commandInput.empty()) return true;

            commandHistory.push_back(">" + commandInput);

            if (commandInput == ":clear") {
                commandHistory.clear();
            }
            else if (commandInput == ":exit" || commandInput == ":q") {
                *activeTab = 0;
            }

            commandInput.clear();

            screen.Post(Event::Custom);
            return true;
        }
        return false;
    });
}