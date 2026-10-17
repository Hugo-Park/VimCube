#ifndef VIMCUBE_APP_H
#define VIMCUBE_APP_H
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/terminal.hpp>
#include <vector>
#include <cstdint>
#include "../geometry/SceneGeometry.h"
#include "../geometry/GeoFactory.h"
#include "../render/GeoDraw.h"
#include "../render/Camera.h"
#include "../utility/Math.h"
#include "KeyBinding.h"
#include "InputCommands.h"

namespace vimcube::interface {
    class VimCubeApp
    {
    private:
        int activeTab = 0;
        KeyBinding keyBinding;
        InputCommands inputCommands;

        std::vector<vimcube::geometry::Line3d> axisIndicator;
        std::vector<vimcube::geometry::Line3d> gridIndicator;
        bool hasAxisIndicator = true;
        bool hasGrid = true;

        std::vector<vimcube::geometry::SceneGeometry> sceneGeos;
        uint32_t nextId = 0;

        vimcube::camera::Camera mainCamera{
            vimcube::camera::ProjectionMode::ISOMETRIC,
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f }, 0.05f
        }; // This Camera object will exist until the program ends

        // For snapshot features
        std::vector<vimcube::geometry::SceneGeometry> savedSceneGeos;
        uint32_t savedId = 0;
        bool hasSnapshot = false;

        bool isFocusMode = false;

    public:
        VimCubeApp();
        vimcube::camera::Camera& getCamera();
        void setActiveTab(int tabNum);
        ftxui::Component createTitleScreen(ftxui::ScreenInteractive& screen);
        ftxui::Component createWorkSpace(ftxui::ScreenInteractive& screen);
        ftxui::Component createTab(ftxui::ScreenInteractive& screen);

        uint32_t addGeoToScene(const vimcube::geometry::Mesh& mesh);
        void removeGeoFromScene(uint32_t id);
        void removeAllGeoFromScene();
        bool selectGeo(uint32_t id, bool moveCamera);
        bool selectNextGeo();
        bool selectPrevGeo();
        void deselectGeo(uint32_t id);
        void deselectGeo();
        void clearSelectGeo();
        vimcube::geometry::SceneGeometry* getSelectedGeo();

        void saveSnapshot();
        void restoreSnapshot();
        void discardSnapshot();

        bool getHasAxisIndicator() const;
        void setHasAxisIndicator(const bool b);
        bool getHasGrid() const;
        void setHasGrid(const bool b);

        bool getIsFocusMode() const;
        void setFocusMode(const bool b);
        void focusOnSelected();
        std::string getCurrentMode() const;
    };
}
#endif
