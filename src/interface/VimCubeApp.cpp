#include "VimCubeApp.h"
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <string>
#include <vector>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/terminal.hpp>
#include "../geometry/GeoFactory.h"
#include "../render/GeoDraw.h"
#include "../interface/InputCommands.h"
#include "../interface/KeyBinding.h"
#include "../utility/Math.h"

namespace vimcube::interface {
    /*
        Function Name : VimCubeApp
        Parameters : -
        Return Type : -
        Description : Constructor for VimCubeApp object
    */
    VimCubeApp::VimCubeApp()
    {
        this->keyBinding.setPreDefinedKeyBinding(*this);
        this->inputCommands.setPredefinedCommands(*this);
    }

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
        Parameters : ftxui::ScreenInteractive& screen
        Return Type : ftxui::Component
        Description : Create title screen by rendering menu(left) and title(right)
    */
    ftxui::Component VimCubeApp::createTitleScreen(ftxui::ScreenInteractive& screen)
    {
        using namespace ftxui;

        static std::vector<std::string> entries {
            "(N)ew File",
            "(O)pen File",
            "(B)uy Me a Coffee",
            "(Q)uit"
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
                #if defined(__APPLE__) && defined(__MACH__)
                    std::system("open https://buymeacoffee.com/huyeong");
                #elif defined(__linux__)
                    std::system("xdg-open https://buymeacoffee.com/huyeong");
                #elif defined(_WIN32) || defined(_WIN64)
                    ShellExecuteA(NULL, "open", https://buymeacoffee.com/huyeong, NULL, NULL, SW_SHOWNORMAL);
                #else
                    return;
                #endif
                return;
            }

            else if (selected == 3)
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

            int leftWidth = width * 0.8;
            int rightWidth = width * 0.2;
            return hbox ({
                leftComponent->Render() | borderDashed | size(WIDTH, EQUAL, leftWidth) | flex,
                rightMenu->Render() | borderDashed | size(WIDTH, EQUAL, rightWidth) | flex,
            }) | flex;
        });

        // Keyboard event handling
        auto componentWithEvents = CatchEvent(finalLayout, [&](Event event) {
            if (event == Event::Character('q')  || event == Event::Character('Q') || event == Event::Escape)
            {
                screen.Exit();
                return true;
            }
            else if (event == Event::Character('n') || event == Event::Character('N'))
            {
                this->activeTab = 1;
            }
            else if (event == Event::Character('o') || event == Event::Character('O'))
            {
                this->activeTab = 1;
            }
            else if (event == Event::Character('b') || event == Event::Character('B'))
            {
                #if defined(__APPLE__) && defined(__MACH__)
                    std::system("open https://buymeacoffee.com/huyeong");
                #elif defined(__linux__)
                    std::system("xdg-open https://buymeacoffee.com/huyeong");
                #else
                    return true;
                #endif
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

        // static std::string commandInput = "";

        InputOption style = InputOption::Default();

        style.transform = [](InputState state) {
            Element e = state.element;

            e |= color(Color::LightGreenBis);
            e |= bgcolor(Color::GrayDark);
            return e;
        };

        style.insert = false; // for blinking block cursor

        // Create input box
        auto inputBox = Input(&commandInput, style);
        auto rightShell = Renderer(inputBox, [=] {

        // Compute file size
        std::string fileSizeStr = "0.0B";
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        if (this->getSelectedGeo() != nullptr)
        {
            float meshSize = this->getSelectedGeo()->getMesh().computeFileSize();
            if (meshSize >= 1024.0f)
            {
                ss << (meshSize * 0.001f) << " KB";
            }
            else
            {
                ss << meshSize << " B";
            }

        }
        else ss << 0.0 << "B";
        fileSizeStr = ss.str();

        return vbox({
            // First Box
            text(" ") | size(HEIGHT, EQUAL, ftxui::Terminal::Size().dimy / 3 - 1),
            separatorDashed(),

            // Second Box
            (this->getSelectedGeo() == nullptr ? 
            text("No Selected Geometry") | color(ftxui::Color::GrayDark) :
            vbox ({
                paragraph("Geo ID: " + std::to_string(this->getSelectedGeo()->getId())),
                paragraph("Vertices: " + std::to_string(this->getSelectedGeo()->getMesh().getVertices().size())),
                paragraph("Edges: " + std::to_string(this->getSelectedGeo()->getMesh().getEdges().size())),
                paragraph("Faces: " + std::to_string(this->getSelectedGeo()->getMesh().getFaces().size())),
                paragraph("File Size: " + fileSizeStr),
                paragraph("Gene Code: " + this->getSelectedGeo()->getSequenceRecord())
            })
            ) | size(HEIGHT, EQUAL, ftxui::Terminal::Size().dimy / 3 - 1),
            separatorDashed(),

            // Third box
            text(keyBinding.getStatusMessage()) | color(Color::Red) | size(HEIGHT, EQUAL, ftxui::Terminal::Size().dimy / 3 - 2),    // FOR ERROR PRINT
            (commandInput.empty() ? text(this->keyBinding.printKeyBuffer()) | bold | color(Color::Yellow1) : inputBox->Render()) | size(HEIGHT, EQUAL, 2)
            }) | borderDashed;
        });

        // Render right canvas
        auto leftViewport = Renderer([this] {
            // Calculate canvas size everytime
            auto terminalSize = ftxui::Terminal::Size();

            int canvasWidth = terminalSize.dimx * 0.8 * 2;
            int canvasHeight = (terminalSize.dimy - 2) * 4;

            auto c = ftxui::Canvas(canvasWidth, canvasHeight);
            
            vimcube::math::Matrix4by4 m;
            vimcube::math::constructIdentityMatrix4by4(m);

            // Draw Grid
            if (this->hasGrid)
            {
                this->gridIndicator = vimcube::geo_factory::createGridIndicator(5);
                vimcube::geo_draw::draw(m, this->mainCamera, this->gridIndicator, c, ftxui::Color::GrayDark);
            }

            // Draw Scene Geometries
            for (auto& obj : this->scene.sceneGeos)
            {
                vimcube::geo_draw::draw(obj.getTransform().getModelMatrix(), this->mainCamera, obj.getMesh(), c, obj.getIsSelected() ? ftxui::Color(ftxui::Color::Yellow3Bis) : ftxui::Color::White);
            }

            // Canvas for axis screen
            auto x = ftxui::Canvas(terminalSize.dimx * 0.15, terminalSize.dimx * 0.15);


            // Draw Axis Indicator
            if (this->hasAxisIndicator)
            {
                // Set Camera for axis indicator
                vimcube::camera::Camera axisCamera(vimcube::camera::ProjectionMode::ISOMETRIC, {0, 0, 0}, this->mainCamera.getUp(), this->mainCamera.getAzimuth(), this->mainCamera.getElevation(), 5.0f );
                this->axisIndicator = vimcube::geo_factory::createAxisIndicator(8);
                std::vector<ftxui::Color> axisColors = {ftxui::Color::Red3Bis, ftxui::Color::Blue3Bis, ftxui::Color::LightGreenBis};
                vimcube::geo_draw::draw(m, axisCamera, this->axisIndicator, x, axisColors);    // Draw axis indicator
                auto leftOffset = emptyElement() | size(WIDTH, EQUAL, 1);
                auto bottomOffset = emptyElement() | size(HEIGHT, EQUAL, 1);

                return vbox ({
                    dbox({ canvas(std::move(c)) | center | borderDashed, 
                        vbox({ filler(), 
                                hbox({ leftOffset,
                                canvas(std::move(x)) 
                                | size(WIDTH, EQUAL, terminalSize.dimx * 0.15 / 2) 
                                | size(HEIGHT, EQUAL, terminalSize.dimx * 0.15 / 4) 
                                | borderDashed | color(Color::GrayDark)}),
                                bottomOffset
                        })
                    }),
                    hbox ({
                        text("STATUS BAR") | size(HEIGHT, EQUAL, 2),
                        filler(),
                        text(this->getCurrentMode()) | color(Color::LightGreenBis) | align_right | size(HEIGHT, EQUAL, 2)    // FOR MODE INFO
                    })
                });
            }
            else 
                return vbox ({
                    canvas(std::move(c)) | center | borderDashed,
                    hbox ({
                        text("STATUS BAR") | size(HEIGHT, EQUAL, 2),
                        filler(),
                        text(this->getCurrentMode()) | color(Color::LightGreenBis) | align_right | size(HEIGHT, EQUAL, 2)    // FOR MODE INFO
                    })
                });
        });

        // Render all layouts
        auto mainLayout = Renderer(rightShell, [=] {
            int width = ftxui::Terminal::Size().dimx;

            int leftWidth = width * 0.8;
            int rightWidth = width * 0.2;

            return hbox({
                leftViewport->Render() | size(WIDTH, EQUAL, leftWidth) | flex,
                rightShell->Render() | size(WIDTH, EQUAL, rightWidth) | flex
            }) | flex;
        });

        // return CatchEvent()
        return CatchEvent(mainLayout, [this](Event event) {
            
            if (this->appMode == AppMode::SEQUENCE)
            {
                keyBinding.runKeyBinding(event);
                return true;
            }

            if (event == Event::Character(':') && this->appMode == AppMode::NORMAL)
            {
                this->appMode = AppMode::COMMAND;
                return false;
            }

            // Enter = run command
            if (event == Event::Return && this->appMode == AppMode::COMMAND) {
                if (commandInput.empty()) return true;
                
                if (commandInput[0] == ':')
                {
                    inputCommands.runCommand(commandInput); // run predifined command
                }

                commandInput.clear();
                this->appMode = AppMode::NORMAL;
                return true;
            }
            
            // ESC = clear input
            if (event == Event::Escape && this->appMode == AppMode::COMMAND) {
                commandInput.clear();
                this->appMode = AppMode::NORMAL;
                return true;
            }

            // ESC = Deselect geometry
            if (event == Event::Escape) {
                if (this->appMode == AppMode::NORMAL || this->isFocusMode)
                {
                    deselectGeo();
                    return true;
                }
                return false;
            }

            // Backspace all = normal mode
            if (event == Event::Backspace && commandInput.size() <= 1)
            {
                this->appMode = AppMode::NORMAL;
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
        uint32_t tempId = this->scene.nextId;
        this->scene.sceneGeos.emplace_back(vimcube::geometry::SceneGeometry(this->scene.nextId++, mesh));
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
        auto it = std::find_if(this->scene.sceneGeos.begin(), this->scene.sceneGeos.end(), [&](const vimcube::geometry::SceneGeometry& geo) { return geo.getId() == id; });
        if (it == this->scene.sceneGeos.end()) return;
        this->scene.sceneGeos.erase(it);
    }

    /*
        Function Name : removeAllGeoFromScene
        Parameters : -
        Return Type : void
        Description : Remove all SceneGeometry
    */
    void VimCubeApp::removeAllGeoFromScene()
    {
        this->scene.sceneGeos.clear();
    }

    /*
        Function Name : selectGeo
        Parameters : uint32_t id, bool moveCamera
        Return Type : bool
        Description : Select geometry
    */
    bool VimCubeApp::selectGeo(uint32_t id, bool moveCamera)
    {
        // moveCamera is true when user selects geometry, not automatically
        bool found = false;
        for (auto& obj : this->scene.sceneGeos)
        {
            if (obj.getId() == id)
            {
                found = true;
                break;
            }
        }

        if (!found) return false;
        for (auto& obj : this->scene.sceneGeos)
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

        if (moveCamera && this->isFocusMode)
        {
            focusOnSelected();
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
        if (this->scene.sceneGeos.empty()) return false;
        auto it = std::find_if(this->scene.sceneGeos.begin(), this->scene.sceneGeos.end(), [&](const vimcube::geometry::SceneGeometry& geo) { return geo.getIsSelected() == true; });
        if (it == this->scene.sceneGeos.end())
        {
            selectGeo(this->scene.sceneGeos.begin()->getId(), true);
            return true;
        }
        size_t idx = std::distance(this->scene.sceneGeos.begin(), it);
        size_t nextIdx = (idx + 1) % this->scene.sceneGeos.size();
        selectGeo(this->scene.sceneGeos[nextIdx].getId(), true);

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
        if (this->scene.sceneGeos.empty()) return false;
        auto it = std::find_if(this->scene.sceneGeos.begin(), this->scene.sceneGeos.end(), [&](const vimcube::geometry::SceneGeometry& geo) { return geo.getIsSelected() == true; });
        if (it == this->scene.sceneGeos.end())
        {
            selectGeo(this->scene.sceneGeos.back().getId(), true);
            return true;
        }
        size_t idx = std::distance(this->scene.sceneGeos.begin(), it);
        size_t nextIdx = (idx + this->scene.sceneGeos.size() - 1) % this->scene.sceneGeos.size();
        selectGeo(this->scene.sceneGeos[nextIdx].getId(), true);

        return true;
    }

    /*
        Function Name : deselectGeo
        Parameters : uint32_t id
        Return Type : void
        Description : Deselect geometry by ID
    */
    void VimCubeApp::deselectGeo(uint32_t id)
    {
        for (auto& obj : this->scene.sceneGeos)
        {
            if (obj.getId() == id)
            {
                obj.setIsSelected(false);
            }
        }
    }

    /*
        Function Name : deselectGeo
        Parameters : -
        Return Type : void
        Description : Deselect geometry
    */
    void VimCubeApp::deselectGeo()
    {
        for (auto& obj : this->scene.sceneGeos)
        {
            if (obj.getIsSelected())
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
        for (auto& obj : this->scene.sceneGeos)
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
        for (auto& obj : this->scene.sceneGeos)
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
        this->savedScene = this->scene;
    }

    /*
        Function Name : restoreSnapshot
        Parameters : -
        Return Type : void
        Description : Restore snapshot
    */
    void VimCubeApp::restoreSnapshot()
    {
        if (this->savedScene.has_value())
        {
            this->scene = this->savedScene.value();
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
        this->savedScene.reset();
    }

    /*
        Function Name : getHasAxisIndicator
        Parameters : -
        Return Type : bool
        Description : Return hasAxisIndicator
    */
    bool VimCubeApp::getHasAxisIndicator() const
    {
        return this->hasAxisIndicator;
    }

    /*
        Function Name : setHasAxisIndicator
        Parameters : const bool b
        Return Type : void
        Description : Set hasAxisIndicator
    */
    void VimCubeApp::setHasAxisIndicator(const bool b)
    {
        this->hasAxisIndicator = b;
    }

    /*
        Function Name : getHasGrid
        Parameters : -
        Return Type : bool
        Description : Return hasGrid
    */
    bool VimCubeApp::getHasGrid() const
    {
        return this->hasGrid; 
    }

    /*
        Function Name : setHasGrid
        Parameters : const bool b
        Return Type : void
        Description : Set hasGrid
    */
    void VimCubeApp::setHasGrid(const bool b)
    {
        this->hasGrid = b;
    }

    /*
        Function Name : getAppMode
        Parameters : -
        Return Type : -
        Description : Get AppMode
    */
    AppMode VimCubeApp::getAppMode() const
    {
        return this->appMode;
    }

    /*
        Function Name : setAppMode
        Parameters : AppMode mode
        Return Type : -
        Description : Set AppMode
    */
    void VimCubeApp::setAppMode(AppMode mode)
    {
        this->appMode = mode;
    }

    /*
        Function Name : getIsFocusMode
        Parameters : -
        Return Type : bool
        Description : Return isFocusMode
    */
    bool VimCubeApp::getIsFocusMode() const
    {
        return this->isFocusMode;
    }

    /*
        Function Name : setFocusMode
        Parameters : const bool b
        Return Type : void
        Description : Set isFocusMode
    */
    void VimCubeApp::setFocusMode(const bool b)
    {
        this->isFocusMode = b;
        if (b)
        {
            focusOnSelected();
        }
    }

    /*
        Function Name : focusOnSelected
        Parameters : -
        Return Type : void
        Description : Set mainCamera target to the center of the bounding box for focus mode
    */
    void VimCubeApp::focusOnSelected()
    {
        vimcube::geometry::SceneGeometry* selectedGeoPtr = this->getSelectedGeo();
        if (selectedGeoPtr != nullptr)
        {
            vimcube::geometry::BoundingBox3d bbox = selectedGeoPtr->getMesh().computeBoundingBox(selectedGeoPtr->getTransform().getModelMatrix());
            vimcube::geometry::Point3d centerPt = bbox.getCenter();
            this->mainCamera.setTarget({centerPt.x, centerPt.y, centerPt.z});

            // float R = std::abs(vimcube::math::getDistance(bbox.max, bbox.min)) / 2.0f;

            float R = selectedGeoPtr->getMesh().getMaxDistanceFromCenter(bbox, selectedGeoPtr->getTransform().getModelMatrix());

            if (this->mainCamera.getProjectionMode() == vimcube::camera::ProjectionMode::ISOMETRIC && R != 0)
            {
                float canvasWidth = ftxui::Terminal::Size().dimx * 0.6 * 2;
                float canvasHeight = (ftxui::Terminal::Size().dimy - 4) * 4;
                if (canvasWidth != 0 && canvasHeight != 0)
                {
                    float aspect = canvasWidth / canvasHeight;
                    this->mainCamera.setZoom(std::min(1.0f, aspect) / R * 0.9f);
                }
            }
            mainCamera.updateCameraPosition();
        }
    }

    /*
        Function Name : getCurrentMode
        Parameters : -
        Return Type : -
        Description : Get current AppMode as string
    */
    std::string VimCubeApp::getCurrentMode() const
    {
        if (this->appMode == AppMode::COMMAND)
            return "-- COMMAND --";
        else if (this->appMode == AppMode::SEQUENCE)
            return "-- SEQUENCE --";
        else if (this->isFocusMode)
            return "-- FOCUS --";
        else
            return "-- NORMAL --";
    }

    /*
        Function Name : setNextId
        Parameters : uint32_t id
        Return Type : void
        Description : Set next id of Scene
    */
    void VimCubeApp::setNextId(uint32_t id)
    {
        this->scene.nextId = id;
    }

    /*
        Function Name : resetApp
        Parameters : -
        Return Type : void
        Description : Reset app
    */
    void VimCubeApp::resetApp()
    {
        this->appMode = AppMode::NORMAL;
        this->hasAxisIndicator = true;
        this->hasGrid = true;
        this->scene = {};
        this->mainCamera.resetCamera();
        this->savedScene = {};
        this->isFocusMode = false;
        this->keyBinding.reset();
        this->inputCommands.reset();
        this->commandInput.clear();
    }
}
