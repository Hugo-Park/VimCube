#include <iostream>
#include "interface/VimCubeApp.h"
using namespace std;
using namespace ftxui;
using namespace vimcube::interface;

int main()
{
    auto screen = ScreenInteractive::FullscreenAlternateScreen();
    VimCubeApp vimCube;

    screen.Loop(vimCube.createTab(screen));

    return 0;
}