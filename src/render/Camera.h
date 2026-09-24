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

        // Projection Mode
        vimcube::camera::ProjectionMode projectionMode_ = vimcube::camera::ProjectionMode::ISOMETRIC;

        // Basic Info
        vimcube::geometry::Vector3d camPosition_;
        vimcube::geometry::Vector3d target_;
        vimcube::geometry::Vector3d up_;

        // For Orbit Rotating
        float azimuth_ = 0.0f;
        float elevation_ = 0.0f;
        float radius_ = 20.0f;

        // For Isometric Mode
        float zoom_ = 10.0f;
        float zoomMin_ = 0.1f;
        float zoomMax_ = 100.f;

        // For Perspective Mode
        float fieldOfView_ = PI / 2;
        float nearClip_ = 0.1f;
        float farClip_ = 100.0f;

    public:
        // Constructor
        Camera(const vimcube::camera::ProjectionMode& projectionMode, const vimcube::geometry::Vector3d& camPosition, const vimcube::geometry::Vector3d& target, const vimcube::geometry::Vector3d& up) : projectionMode_(projectionMode), camPosition_(camPosition), target_(target), up_(up) {};

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
