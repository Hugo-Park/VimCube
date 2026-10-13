#include "interface/VimCubeApp.h"

int main()
{
    auto screen = ftxui::ScreenInteractive::FullscreenAlternateScreen();
    vimcube::interface::VimCubeApp app;

    screen.Loop(app.createTab(screen));

    return 0;
}
