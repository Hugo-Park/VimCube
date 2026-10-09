#include "Transform.h"

namespace vimcube::geometry {

    /*
        Function Name : getModelMatrix
        Parameters : -
        Return Type : vimcube::math::Matrix4by4
        Description : Get model matrix by T * R * S production
    */
    vimcube::math::Matrix4by4 Transform::getModelMatrix() const
    {
        vimcube::math::Matrix4by4 T = vimcube::math::buildMoveMatrix(this->position_.x, this->position_.y, this->position_.z);
        vimcube::math::Matrix4by4 S = vimcube::math::buildScaleMatrix(this->scale_.x, this->scale_.y, this->scale_.z);
        vimcube::math::Matrix4by4 rx = vimcube::math::buildRotateMatrix(vimcube::math::Axis::X, this->rotation_.x);
        vimcube::math::Matrix4by4 ry = vimcube::math::buildRotateMatrix(vimcube::math::Axis::Y, this->rotation_.y);
        vimcube::math::Matrix4by4 rz = vimcube::math::buildRotateMatrix(vimcube::math::Axis::Z, this->rotation_.z);

        // RZ * RY * RZ
        vimcube::math::Matrix4by4 R = vimcube::math::getProduct4by4(rz, vimcube::math::getProduct4by4(ry, rx));

        // T * R * S
        return vimcube::math::getProduct4by4(T, vimcube::math::getProduct4by4(R, S));
    }

    /*
        Function Name : getPosition
        Parameters : -
        Return Type : const vimcube::geometry::Point3d
        Description : Return position
    */
    const vimcube::geometry::Point3d Transform::getPosition() const
    {
        return this->position_;
    }

    /*
        Function Name : getScale
        Parameters : -
        Return Type : const vimcube::geometry::Vector3d
        Description : Return scale
    */
    const vimcube::geometry::Vector3d Transform::getScale() const
    {
        return this->scale_;
    }
    
    /*
        Function Name : getRotation
        Parameters : -
        Return Type : const vimcube::geometry::Vector3d
        Description : Return rotation
    */
    const vimcube::geometry::Vector3d Transform::getRotation() const
    {
        return this->rotation_;
    }

    /*
        Function Name : setPosition
        Parameters : float tx, float ty, float tz
        Return Type : void
        Description : Set move position
    */
    void Transform::setPosition(float tx, float ty, float tz)
    {
        this->position_.x = tx;
        this->position_.y = ty;
        this->position_.z = tz;
    }

    /*
        Function Name : setScale
        Parameters : float sx, float sy, float sz
        Return Type : void
        Description : Set scale factor
    */
    void Transform::setScale(float sx, float sy, float sz)
    {
        if (sx == 0.0f || sy == 0.0f || sz == 0.0f) return;
        this->scale_.x = sx;
        this->scale_.y = sy;
        this->scale_.z = sz;
    }

    /*
        Function Name : setRotation
        Parameters : vimcube::math::Axis axis, float theta
        Return Type : void
        Description : Set rotation axis and angle(radians)
    */
    void Transform::setRotation(vimcube::math::Axis axis, float theta)
    {
        if (axis == vimcube::math::Axis::X)
        {
            this->rotation_.x = theta;
        }
        else if (axis == vimcube::math::Axis::Y)
        {
            this->rotation_.y = theta;
        }
        else if (axis == vimcube::math::Axis::Z)
        {
            this->rotation_.z = theta;
        }
    }

    /*
        Function Name : translate
        Parameters : vimcube::math::Axis axis, float amount
        Return Type : void
        Description : Accumulate position
    */
    void Transform::translate(vimcube::math::Axis axis, float amount)
    {
        if (axis == vimcube::math::Axis::X)
        {
            this->position_.x += amount;
        }
        else if (axis == vimcube::math::Axis::Y)
        {
            this->position_.y += amount;
        }
        else if (axis == vimcube::math::Axis::Z)
        {
            this->position_.z += amount;
        }
    }

    /*
        Function Name : scale
        Parameters : vimcube::math::Axis axis, float factor
        Return Type : void
        Description : Accumulate scale factor
    */
    void Transform::scale(vimcube::math::Axis axis, float factor)
    {
        if (factor == 0.0f) return;
        if (axis == vimcube::math::Axis::X)
        {
            this->scale_.x *= factor;
        }
        else if (axis == vimcube::math::Axis::Y)
        {
            this->scale_.y *= factor;
        }
        else if (axis == vimcube::math::Axis::Z)
        {
            this->scale_.z *= factor;
        }
    }

    /*
        Function Name : rotate
        Parameters : vimcube::math::Axis axis, float theta
        Return Type : void
        Description : Accumulate rotation angle
    */
    void Transform::rotate(vimcube::math::Axis axis, float theta)
    {
        if (axis == vimcube::math::Axis::X)
        {
            this->rotation_.x += theta;
        }
        else if (axis == vimcube::math::Axis::Y)
        {
            this->rotation_.y += theta;
        }
        else if (axis == vimcube::math::Axis::Z)
        {
            this->rotation_.z += theta;
        }
    }
}
