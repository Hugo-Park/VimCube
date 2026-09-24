#ifndef CAMERA_H
#define CAMERA_H
#include "../geometry/GeoTypes.h"
#include "../utility/Math.h"

namespace vimcube::camera {

    enum class ProjectionMode { ISOMETRIC, PERSPECTIVE };

    class Camera {
    private:

        // Define special constants
        static constexpr float EPSILON = 0.01f;
        static constexpr float PI = 3.141592;
        static constexpr float INIT_AZIMUTH = PI / 4.0f;
        static constexpr float INIT_ELEVATION = 0.9553;
        static constexpr float INIT_RADIUS = 20.0f;

        // Projection Mode
        vimcube::camera::ProjectionMode projectionMode_ = vimcube::camera::ProjectionMode::ISOMETRIC;

        // Basic Info
        vimcube::geometry::Vector3d camPosition_;
        vimcube::geometry::Vector3d target_;
        vimcube::geometry::Vector3d up_;

        // For Orbit Rotating
        float azimuth_ = 0.0f;
        float elevation_ = PI / 4.0f;
        float radius_ = 20.0f;

        // For Isometric Mode
        float zoom_ = 0.1f;
        float zoomMin_ = 0.01f;
        float zoomMax_ = 1000.0f;

        // For Perspective Mode
        float fieldOfView_ = PI / 2;
        float nearClip_ = 0.1f;
        float farClip_ = 100.0f;

    public:
        // Constructor
        Camera(const vimcube::camera::ProjectionMode& projectionMode, const vimcube::geometry::Vector3d& target, const vimcube::geometry::Vector3d& up);

        // Setter
        void setProjectionMode(const vimcube::camera::ProjectionMode& projectionMode);
        void setTarget(const vimcube::geometry::Vector3d& target);
        
        // Getter
        const vimcube::camera::ProjectionMode& getProjectionMode() const;
        const vimcube::geometry::Vector3d& getCamPosition() const;
        const vimcube::geometry::Vector3d& getTarget() const;
        float getZoom() const;  // Only for Isometric mode
        float getFieldOfView() const;
        float getNearClip() const;
        float getFarClip() const;

        // Methods
        // angle parameters are always Radian
        void updateCameraPosition();
        void orbitUp(float angle);
        void orbitDown(float angle);
        void orbitLeft(float angle);
        void orbitRight(float angle);

        // Only for isometric mode
        void zoomIn(float amount);
        void zoomOut(float amount);

        // Projection
        vimcube::geometry::Point2d projectToCanvas(const vimcube::geometry::Point3d& worldPt, float canvasWidth, float canvasHeight);

        // Reset
        void resetCamera();
    };

}
#endif
