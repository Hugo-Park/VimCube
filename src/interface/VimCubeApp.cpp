#include "VimCubeApp.h"
#include <cstdint>
#include <algorithm>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
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
                vimcube::geo_draw::draw(obj.getTransform().getModelMatrix(), this->mainCamera, obj.getMesh(), c, obj.getIsSelected() ? ftxui::Color::Yellow : ftxui::Color::White);   // Draw Geometry
            }

            // Canvas for axis screen
            auto x = ftxui::Canvas(terminalSize.dimx * 0.15, terminalSize.dimx * 0.15);

            // Set Camera for axis indicator
            vimcube::camera::Camera axisCamera(vimcube::camera::ProjectionMode::ISOMETRIC, {0, 0, 0}, this->mainCamera.getUp(), this->mainCamera.getAzimuth(), this->mainCamera.getElevation(), 5.0f );
            vimcube::geometry::Mesh axisMesh = vimcube::geo_factory::createAxisIndicator(8);
            vimcube::math::Matrix4by4 m;
            vimcube::math::constructIdentityMatrix4by4(m);
            vimcube::geo_draw::draw(m, axisCamera, axisMesh, x, ftxui::Color::White);    // Draw axis indicator

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
        Parameters : const vimcube::geometry::Mesh& mesh
        Return Type : uint32_t
        Description : Add SceneGeometry
    */
    uint32_t VimCubeApp::addGeoToScene(const vimcube::geometry::Mesh& mesh)
    {
        uint32_t tempId = this->nextId;
        sceneGeos.emplace_back(vimcube::geometry::SceneGeometry(this->nextId++, mesh));
        return tempId;
    }

    /*
        Function Name : removeGeoFromScene
        Parameters : uint32_t id
        Return Type : void
        Description : Remove SceneGeometry
    */
    void VimCubeApp::removeGeoFromScene(uint32_t id)
    {
        auto it = std::find_if(sceneGeos.begin(), sceneGeos.end(), [&](const vimcube::geometry::SceneGeometry& geo) { return geo.getId() == id; });
        if (it == sceneGeos.end()) return;
        sceneGeos.erase(it);
    }

    /*
        Function Name : selectGeo
        Parameters : uint32_t id
        Return Type : bool
        Description : Select geometry
    */
    bool VimCubeApp::selectGeo(uint32_t id)
    {
        bool found = false;
        for (auto& obj : sceneGeos)
        {
            if (obj.getId() == id)
            {
                found = true;
                break;
            }
        }

        if (!found) return false;
        for (auto& obj : sceneGeos)
        {
            if (obj.getId() == id)
            {
                obj.setIsSelected(true);
            }
            else
            {
                obj.setIsSelected(false);
            }
        }
        return true;
    }

    /*
        Function Name : selectNextGeo
        Parameters : -
        Return Type : bool
        Description : Select next geometry
    */
    bool VimCubeApp::selectNextGeo()
    {
        if (this->sceneGeos.empty()) return false;
        auto it = std::find_if(sceneGeos.begin(), sceneGeos.end(), [&](const vimcube::geometry::SceneGeometry& geo) { return geo.getIsSelected() == true; });
        if (it == sceneGeos.end())
        {
            selectGeo(sceneGeos.begin()->getId());
            return true;
        }
        size_t idx = std::distance(sceneGeos.begin(), it);
        size_t nextIdx = (idx + 1) % sceneGeos.size();
        selectGeo(sceneGeos[nextIdx].getId());
        return true;
    }

    /*
        Function Name : selectPrevGeo
        Parameters : -
        Return Type : bool
        Description : Select previous geometry
    */
    bool VimCubeApp::selectPrevGeo()
    {
        if (this->sceneGeos.empty()) return false;
        auto it = std::find_if(sceneGeos.begin(), sceneGeos.end(), [&](const vimcube::geometry::SceneGeometry& geo) { return geo.getIsSelected() == true; });
        if (it == sceneGeos.end())
        {
            selectGeo(sceneGeos.back().getId());
            return true;
        }
        size_t idx = std::distance(sceneGeos.begin(), it);
        size_t nextIdx = (idx + sceneGeos.size() - 1) % sceneGeos.size();
        selectGeo(sceneGeos[nextIdx].getId());
        return true;
    }

    /*
        Function Name : deselectGeo
        Parameters : -
        Return Type : void
        Description : Deselect geometry
    */
    void VimCubeApp::deselectGeo(uint32_t id)
    {
        for (auto& obj : sceneGeos)
        {
            if (obj.getId() == id)
            {
                obj.setIsSelected(false);
            }
        }
    }

    /*
        Function Name : clearSelectGeo
        Parameters : -
        Return Type : void
        Description : Deselect all geometry
    */
    void VimCubeApp::clearSelectGeo()
    {
        for (auto& obj : sceneGeos)
        {
            obj.setIsSelected(false);
        }
    }

    /*
        Function Name : getSelectedGeo
        Parameters : -
        Return Type : vimcube::geometry::SceneGeometry*
        Description : Get pointer of selected geometry. Use this method everytime before calling transform commands because returned pointer would be invalid after addGeoToScene().
    */
    vimcube::geometry::SceneGeometry* VimCubeApp::getSelectedGeo()
    {
        for (auto& obj : sceneGeos)
        {
            if (obj.getIsSelected())
                return &obj;
        }
        return nullptr;
    }

    /*
        Function Name : saveSnapshot
        Parameters : -
        Return Type : void
        Description : Save snapshot
    */
    void VimCubeApp::saveSnapshot()
    {
        this->savedSceneGeos = this->sceneGeos;
        this->savedId = this->nextId;
        this->hasSnapshot = true;
    }

    /*
        Function Name : restoreSnapshot
        Parameters : -
        Return Type : void
        Description : Restore snapshot
    */
    void VimCubeApp::restoreSnapshot()
    {
        if (hasSnapshot)
        {
            this->sceneGeos = this->savedSceneGeos;
            this->nextId = this->savedId;
        }
    }

    /*
        Function Name : discardSnapshot
        Parameters : -
        Return Type : void
        Description : Discard snapshot
    */
    void VimCubeApp::discardSnapshot()
    {
        this->hasSnapshot = false;
        this->savedSceneGeos.clear();
    }
}
