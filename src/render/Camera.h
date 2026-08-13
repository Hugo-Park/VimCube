#ifndef CAMERA_H
#define CAMERA_H
#include "../geometry/GeoTypes.h"

namespace vimcube::camera {

    enum class ProjectionMode { ISOMETRIC, PERSPECTIVE };

    class Camera {
    private:

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

        // For Perspective Mode
        float fieldOfView_ = 90.0f;
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

        // Methods
        void updateCameraPosition();
        void orbitUp(float angle);
        void orbitDown(float angle);
        void orbitLeft(float angle);
        void orbitRight(float angle);

        // Only for isometric mode
        void zoomIn(float amount);
        void zoomOut(float amount);

        // Projection
        void projectToCanvas(const vimcube::geometry::Point3d& worldPt, float canvasWidth, float canvasHeight);

        // Reset
        void resetCamera();
    };

}
#endif