#ifndef VIMCUBE_APP_H
#define VIMCUBE_APP_H
#include <ftxui/component/app.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/terminal.hpp>
#include <vector>
#include <cstdint>
#include <optional>
#include "../geometry/SceneGeometry.h"
#include "../geometry/Scene.h"
#include "../render/Camera.h"
#include "../io/FileManager.h"
#include "KeyBinding.h"
#include "InputCommands.h"
#include "SequenceRunner.h"

namespace vimcube::interface {

    enum class AppMode { NORMAL, SEQUENCE, COMMAND };

    class VimCubeApp
    {
    private:
        int activeTab = 0;
        int panelIndex = 0;
        AppMode appMode = AppMode::NORMAL;
        KeyBinding keyBinding;
        InputCommands inputCommands;
        std::string commandInput;

        std::vector<vimcube::geometry::Line3d> axisIndicator;
        std::vector<vimcube::geometry::Line3d> gridIndicator;
        bool hasAxisIndicator = true;
        bool hasGrid = true;

        vimcube::geometry::Scene scene;
        std::optional<vimcube::geometry::Scene> savedScene;

        vimcube::camera::Camera mainCamera{
            vimcube::camera::ProjectionMode::ISOMETRIC,
            { 0.0f, 0.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f }, 0.05f
        }; // This Camera object will exist until the program ends

        bool isFocusMode = false;

        vimcube::file::FileManager fileManager;
        std::string openFileName;

        std::string appMessage;

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

        AppMode getAppMode() const;
        void setAppMode(AppMode mode);

        bool getIsFocusMode() const;
        void setFocusMode(const bool b);
        void focusOnSelected();
        std::string getCurrentMode() const;

        vimcube::geometry::Scene& getScene();
        void setNextId(uint32_t id);

        vimcube::file::FileManager& getFileManager();

        RunnerResult applyLoadedGeometries(const std::vector<vimcube::file::GeometryRecord>& records);

        void setAppMessage(const std::string& msg);
        const std::string& getAppMessage() const;

        void resetApp();
    };
}
#endif
