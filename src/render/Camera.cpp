#include "Camera.h"

namespace vimcube::camera {

    // Setter
    void Camera::setProjectionMode(const vimcube::camera::ProjectionMode& projectionMode)
    {
        this->projectionMode_ = projectionMode;
    }

    void Camera::setTarget(const vimcube::geometry::Vector3d& target)
    {
        this->target_ = target;
    }

    // Getter
    const vimcube::camera::ProjectionMode& Camera::getProjectionMode() const
    {
        return this->projectionMode_;
    }

    const vimcube::geometry::Vector3d& Camera::getCamPosition() const
    {
        return this->camPosition_;
    }

    const vimcube::geometry::Vector3d& Camera::getTarget() const
    {
        return this->target_;
    }

    float Camera::getZoom() const
    {
        return this->zoom_;
    }
}