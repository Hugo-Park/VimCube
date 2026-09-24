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


    float Camera::getFieldOfView() const
    {
        return this->fieldOfView_;
    }

    float Camera::getNearClip() const
    {
        return this->nearClip_;
    }
    float Camera::getFarClip() const
    {
        return this->farClip_;
    }

    /*
        Function Name : updateCameraPosition 
        Parameters : -
        Return Type : void
        Description : Update the position of camera object
    */
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

    /*
        Function Name : orbitUp 
        Parameters : float angle
        Return Type : void
        Description : Move up the orbit
    */
    void Camera::orbitUp(float angle)
    {
        this->elevation_ -= angle;
        if (this->elevation_ <= EPSILON)
        {
            this->elevation_ = EPSILON;
        }
        updateCameraPosition();
        
    }

    /*
        Function Name : orbitDown 
        Parameters : float angle
        Return Type : void
        Description : Move down the orbit
    */
    void Camera::orbitDown(float angle)
    {
        this->elevation_ += angle;
        if (this->elevation_ >= PI - EPSILON)
        {
            this->elevation_ = PI - EPSILON;
        }
        updateCameraPosition();
    }

    /*
        Function Name : orbitLeft 
        Parameters : float angle
        Return Type : void
        Description : Move left the orbit
    */
    void Camera::orbitLeft(float angle)
    {
        this->azimuth_ += angle;
        this->azimuth_ = std::fmod(this->azimuth_, 2 * PI);
        if (this->azimuth_ < 0)
        {
            this->azimuth_ += 2 * PI;
        }
        updateCameraPosition();
    }

    /*
        Function Name : orbitRight 
        Parameters : float angle
        Return Type : void
        Description : Move right the orbit
    */
    void Camera::orbitRight(float angle)
    {
        this->azimuth_ -= angle;
        this->azimuth_ = std::fmod(this->azimuth_, 2 * PI);
        if (this->azimuth_ < 0)
        {
            this->azimuth_ += 2 * PI;
        }
        updateCameraPosition();
    }

    /*
        Function Name : zoomIn 
        Parameters : float amount
        Return Type : void
        Description : ISOMETRIC - Set zoom ratio(plus for zoom in) / PERSPECTIVE - Set radius in spherical coordinates(minus for zoom in)
    */
    void Camera::zoomIn(float amount)
    {
        // for ISOMETRIC
        if (this->projectionMode_ == ProjectionMode::ISOMETRIC)
        {
            this->zoom_ += amount;
            if (this->zoom_ >= this->zoomMax_)
            {
                this->zoom_ = this->zoomMax_;
            }
        }

        // for PERSPECTIVE
        else
        {
            this->radius_ -= amount;
            if (this->radius_ <= this->nearClip_)
            {
                this->radius_ = this->nearClip_;
            }
            updateCameraPosition();
        }
    }

    /*
        Function Name : zoomOut 
        Parameters : float amount
        Return Type : void
        Description : ISOMETRIC - Set zoom ratio(minus for zoom out) / PERSPECTIVE - Set radius in spherical coordinates(plus for zoom out)
    */
    void Camera::zoomOut(float amount)
    {
        // for ISOMETRIC
        if (this->projectionMode_ == ProjectionMode::ISOMETRIC)
        {
            this->zoom_ -= amount;
            if (this->zoom_ <= this->zoomMin_)
            {
                this->zoom_ = this->zoomMin_;
            }
        }

        // for PERSPECTIVE
        else
        {
            this->radius_ += amount;
            if (this->radius_ >= this->farClip_)
            {
                this->radius_ = this->farClip_;
            }
            updateCameraPosition();
        }
    }

    vimcube::geometry::Point2d Camera::projectToCanvas(const vimcube::geometry::Point3d& worldPt, float canvasWidth, float canvasHeight)
    {
        float aspect = canvasWidth / canvasHeight;
        vimcube::math::Matrix4by4 viewMatrix = vimcube::math::buildViewMatrix(this->camPosition_, this->target_, this->up_);
        vimcube::math::Matrix4by4 projectionMatrix;
        
        if (this->projectionMode_ == vimcube::camera::ProjectionMode::ISOMETRIC)
        {
            projectionMatrix = vimcube::math::buildOrthogonalMatrix(this->zoom_, aspect, this->nearClip_, this->farClip_);
        }

        else
        {
            projectionMatrix = vimcube::math::buildPerspectiveMatrix(this->fieldOfView_, aspect, this->nearClip_, this->farClip_);
        }

        // Product (Projection matrix * View matrix)
        vimcube::math::Matrix4by4 vpMatrix = vimcube::math::getProduct4by4(projectionMatrix, viewMatrix);

        // Cast worldPt to point4d
        vimcube::geometry::Point4d castPoint4d = vimcube::math::castToPoint4d(worldPt);

        vimcube::geometry::Point4d resultPoint4d = vimcube::math::getProduct4by4andPoint4d(vpMatrix, castPoint4d);

        vimcube::geometry::Point3d resultPoint3d(0, 0, 0);
        resultPoint3d.x = float(resultPoint4d.x / resultPoint4d.w);
        resultPoint3d.y = float(resultPoint4d.y / resultPoint4d.w);
        resultPoint3d.z = float(resultPoint4d.z / resultPoint4d.w);

        // Convert to real canvas pixel coordinates
        float pixelX = (resultPoint3d.x + 1) / 2 * canvasWidth;
        float pixelY = (1 - resultPoint3d.y) / 2 * canvasHeight;

        return vimcube::geometry::Point2d(pixelX, pixelY);
    }
}    // namespace vimcube::camera
