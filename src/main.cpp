#include <iostream>
#include "VimCube-title.h"
using namespace std;
using namespace ftxui;

int main()
{
    auto screen = ScreenInteractive::FullscreenAlternateScreen();
    int activeTab = 0;

    auto titleScreen = createTitleScreen(&activeTab, screen);
    auto workSpace = createWorkSpace(&activeTab, screen);

    auto mainTab = Container::Tab({
        titleScreen,
        workSpace
    }, &activeTab);

    screen.Loop(mainTab);

    return 0;
}
