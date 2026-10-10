#ifndef VIMCUBE_APP_H
#define VIMCUBE_APP_H
#include <cstdint>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/terminal.hpp>
#include <vector>
#include "../geometry/SceneGeometry.h"
#include "../geometry/GeoFactory.h"
#include "../render/GeoDraw.h"
#include "../render/Camera.h"
#include "../utility/Math.h"

namespace vimcube::interface {
    class VimCubeApp
    {
    private:
        int activeTab = 0;
        vimcube::geometry::Mesh axisIndicator;
        std::vector<vimcube::geometry::SceneGeometry> sceneGeos;
        uint32_t nextId = 0;

        vimcube::camera::Camera mainCamera{
            vimcube::camera::ProjectionMode::ISOMETRIC,
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f }
        }; // This Camera object will exist until the program ends

        // For snapshot features
        std::vector<vimcube::geometry::SceneGeometry> savedSceneGeos;
        uint32_t savedId = 0;
        bool hasSnapshot = false;

    public:
        VimCubeApp();
        vimcube::camera::Camera& getCamera();
        void setActiveTab(int tabNum);
        ftxui::Component createTitleScreen(ftxui::ScreenInteractive& screen);
        ftxui::Component createWorkSpace(ftxui::ScreenInteractive& screen);
        ftxui::Component createTab(ftxui::ScreenInteractive& screen);

        uint32_t addGeoToScene(const vimcube::geometry::Mesh& mesh);
        void removeGeoFromScene(uint32_t id);
        bool selectGeo(uint32_t id);
        bool selectNextGeo();
        bool selectPrevGeo();
        void deselectGeo(uint32_t id);
        void clearSelectGeo();
        vimcube::geometry::SceneGeometry* getSelectedGeo();

        void saveSnapshot();
        void restoreSnapshot();
        void discardSnapshot();
    };
}
#endif
