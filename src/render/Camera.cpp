#include "Camera.h"
#include <cmath>
namespace vimcube::camera
{

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

    void Camera::updateCameraPosition()
    {
        // Convert spherical coordinates to cartesian coordinates
        float tempX = this->radius_ * sin(this->elevation_) * cos(this->azimuth_);
        float tempY = this->radius_ * sin(this->elevation_) * sin(this->azimuth_);
        float tempZ = this->radius_ * cos(this->elevation_);

        // Update Camera position using target position
        this->camPosition_.x = this->target_.x + tempX;
        this->camPosition_.y = this->target_.y + tempY;
        this->camPosition_.z = this->target_.z + tempZ;
    }
}    // namespace vimcube::camera
