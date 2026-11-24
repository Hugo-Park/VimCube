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

    // Location of the point related to the plane
    enum class PlaneSide { FORWARD, BACKWARD, ON_PLANE };

    TriangleInfo getTriangleInfoFromPoints(const vimcube::geometry::Point3d& p1, const vimcube::geometry::Point3d& p2, const vimcube::geometry::Point3d& p3);

    float computeSignedDistance(const vimcube::geometry::Vector3d& normal, const vimcube::geometry::Point3d& point, const vimcube::geometry::Point3d& pointOnPlane);

    PlaneSide classifySignedDistance(const float d);
}
#endif
