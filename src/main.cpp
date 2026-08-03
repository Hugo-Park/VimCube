#include <iostream>
#include "interface/VimCubeApp.h"
using namespace std;
using namespace ftxui;

int main()
{
    auto screen = ScreenInteractive::FullscreenAlternateScreen();
    VimCubeApp vimCube;

    screen.Loop(vimCube.createTab(screen));

    return 0;
}