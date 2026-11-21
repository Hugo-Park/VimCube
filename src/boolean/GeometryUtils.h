#ifndef GEOMETRY_UTILS_H
#define GEOMETRY_UTILS_H
#include "../geometry/GeoTypes.h"

namespace vimcube::boolean
{
    struct TriangleInfo
    {
        vimcube::geometry::Vector3d normal = {0, 0, 0};
        float area = 0;
        bool isDegenerate = true;
    };

    TriangleInfo getTriangleInfoFromPoints(const vimcube::geometry::Point3d& p1, const vimcube::geometry::Point3d& p2, const vimcube::geometry::Point3d& p3);
}
#endif
