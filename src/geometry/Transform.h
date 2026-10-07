#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "GeoTypes.h"
#include "../utility/Math.h"

namespace vimcube::geometry {

    class Transform
    {
    private:
        vimcube::geometry::Point3d position_ = {0,0,0};
        vimcube::geometry::Vector3d scale_ = {1,1,1};
        vimcube::geometry::Vector3d rotation_ = {0,0,0};

    public:
        vimcube::math::Matrix4by4 getModelMatrix() const;

        const vimcube::geometry::Point3d getPosition() const;
        const vimcube::geometry::Vector3d getScale() const;
        const vimcube::geometry::Vector3d getRotation() const;

        void setPosition(float tx, float ty, float tz);
        void setScale(float sx, float sy, float sz);
        void setRotation(vimcube::math::Axis axis, float theta);
    };
}
#endif
