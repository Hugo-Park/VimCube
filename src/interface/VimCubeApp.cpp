#include "VimCubeApp.h"
#include <ftxui/dom/elements.hpp>
#include "../interface/InputCommands.h"
#include "../interface/KeyBinding.h"

namespace vimcube::interface {
    /*
        Function Name : VimCubeApp
        Parameters : -
        Return Type : -
        Description : Constructor for VimCubeApp object
    */
    VimCubeApp::VimCubeApp() {};

    /*
        Function Name : getCamera
        Parameters : void
        Return Type : vimcube::camera::Camera&
        Description : Return mainCamera object (call-by-reference)
    */
    vimcube::camera::Camera& VimCubeApp::getCamera()
    {
        return this->mainCamera;
    }

    /*
        Function Name : setActiveTab
        Parameters : int tabNum
        Return Type : -
        Description : Set activeTab variable
    */
    void VimCubeApp::setActiveTab(int tabNum)
    {
        activeTab = tabNum;
    }
    /*
        Function Name : createTitleScreen
        Parameters : int* activeTab, ftxui::ScreenInteractive& screen
        Return Type : ftxui::Component
        Description : Create title screen by rendering menu(left) and title(right)
    */
    ftxui::Component VimCubeApp::createTitleScreen(ftxui::ScreenInteractive& screen)
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
        option.on_enter = [&] {
            if (selected == 0)
            {
                activeTab = 1;
                return;
            }

            else if (selected == 1)
            {
                activeTab = 1;
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
    ftxui::Component VimCubeApp::createWorkSpace(ftxui::ScreenInteractive& screen)
    {
        using namespace ftxui;

        static std::string commandInput = "";
        static std::vector<std::string> commandHistory;

        // set KeyBinding object
        static KeyBinding keyBinding;
        static bool isKeyBindingSet = false;
        if (!isKeyBindingSet)
        {
            keyBinding.setPreDefinedKeyBinding(*(this), commandHistory);
            isKeyBindingSet = true;
        }

        // Create input box
        auto inputBox = Input(&commandInput, "...");

        auto rightShell = Renderer(inputBox, [=] {
            Elements historyElements;

            if (commandHistory.empty()) {
                historyElements.push_back(text("Press colon(:)...") | dim);
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
                vbox(std::move(historyElements)) | flex,
                separator(),
                hbox({
                    text("$") | bold,
                    inputBox->Render() | flex,
                    text(keyBinding.printKeyBuffer()) | bold | center | size(WIDTH, EQUAL, 5) | color(Color::Yellow1)
                }),
            }) | border;
        });

        // Render right canvas
        auto leftViewport = Renderer([this] {
            // Calculate canvas size everytime
            auto terminalSize = ftxui::Terminal::Size();

            int canvasWidth = terminalSize.dimx * 0.6 * 2;
            int canvasHeight = (terminalSize.dimy - 4) * 4;

            auto c = ftxui::Canvas(canvasWidth, canvasHeight);
            
            for (auto& obj : this->sceneGeos)
            {
                std::visit([&c, this] (auto& item) {
                    vimcube::geo_draw::draw(this->mainCamera, item, c);   // Draw Geometry
                }, obj);
            }

            // Canvas for axis screen
            auto x = ftxui::Canvas(terminalSize.dimx * 0.15, terminalSize.dimx * 0.15);

            // Set Camera for axis indicator
            vimcube::camera::Camera axisCamera(vimcube::camera::ProjectionMode::ISOMETRIC, {0, 0, 0}, this->mainCamera.getUp(), this->mainCamera.getAzimuth(), this->mainCamera.getElevation(), 5.0f );
            vimcube::geometry::Mesh axisMesh = vimcube::geo_factory::createAxisIndicator(8);
            geo_draw::draw(axisCamera, axisMesh, x);    // Draw axis indicator

            auto leftOffset = emptyElement() | size(WIDTH, EQUAL, 1);
            auto bottomOffset = emptyElement() | size(HEIGHT, EQUAL, 1);

            return dbox({ canvas(std::move(c)) | center | border, 
                    vbox({ filler(), 
                            hbox({ leftOffset,
                            canvas(std::move(x)) 
                            | size(WIDTH, EQUAL, terminalSize.dimx * 0.15 / 2) 
                            | size(HEIGHT, EQUAL, terminalSize.dimx * 0.15 / 4) 
                            | borderDashed }),
                            bottomOffset
                        })
                    });
            // return canvas(std::move(c)) | center | border;
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

        // return CatchEvent()
        return CatchEvent(mainLayout, [=, &screen](Event event) {
            
            // enter key event handling
            if (event == Event::Return) {
                if (commandInput.empty()) return true;
                
                if (commandInput[0] == ':')
                {
                    InputCommands binding;  // construct InputCommands class
                    binding.setPredefinedCommands(*(this), commandHistory);
                    commandHistory.push_back(commandInput);
                    binding.runCommand(commandInput); // run predifined command
                }

                commandInput.clear();
                screen.Post(Event::Custom);
                return true;
            }
            
            // insert string if it has a colon(:) in first character
            if (event.is_character())
            {
                if (commandInput.empty())
                {
                    // if string is not starting with colon(:)
                    if (event.character() != ":") 
                    {
                        keyBinding.runKeyBinding(event); // run key binding
                        return true; // nothing happens on the display
                    }
                }
            }

            return false;
        });

    }

    /*
        Function Name : createTab
        Parameters : ftxui::ScreenInteractive& screen
        Return Type : ftxui::Component
        Description : Constructor for VimCubeApp object
    */
    ftxui::Component VimCubeApp::createTab(ftxui::ScreenInteractive& screen)
    {
        auto titleScreen = this->createTitleScreen(screen);
        auto workSpace = this->createWorkSpace(screen);

        auto mainTab = ftxui::Container::Tab({
            titleScreen,
            workSpace
        }, &activeTab);

        return mainTab;
    }
    
    /*
        Function Name : addGeoToScene 
        Parameters : Geometry& geometry
        Return Type : void
        Description : Add Geometry for drawing.
    */
    void VimCubeApp::addGeoToScene(const Geometry& geometry)
    {
        sceneGeos.push_back(geometry);
    }
}
