#ifndef GEOMETRY_UTILS_H
#define GEOMETRY_UTILS_H
#include "../geometry/GeoTypes.h"
#include "../geometry/Mesh.h"

namespace vimcube::boolean
{
    // Location of the point related to the plane
    enum class PlaneSide { FORWARD, BACKWARD, ON_PLANE };

    // Relation between two triangles
    // NONE : no intersection
    // SEGMENT : line of intersection exists
    // COPLANAR : on the same plane
    // INVALID : one or two triangles is degenerated
    enum class TriangleRelation { NONE, SEGMENT, COPLANAR, INVALID };

    // Relation between point and mesh
    enum class PointRelation { INSIDE, OUTSIDE, UNCERTAIN };

    struct TriangleInfo
    {
        vimcube::geometry::Vector3d normal = {0, 0, 0};
        float area = 0;
        bool isDegenerate = true;
    };

    struct TriangleIntersection
    {
        TriangleRelation relation = TriangleRelation::NONE;
        vimcube::geometry::Point3d start = {0, 0, 0};
        vimcube::geometry::Point3d end = {0, 0, 0};
    };

    vimcube::geometry::Triangle getTriangleFromFace(const vimcube::geometry::Mesh& m, const vimcube::geometry::Face& f);

    TriangleInfo getTriangleInfoFromPoints(const vimcube::geometry::Point3d& p1, const vimcube::geometry::Point3d& p2, const vimcube::geometry::Point3d& p3);

    float computeSignedDistance(const vimcube::geometry::Vector3d& normal, const vimcube::geometry::Point3d& point, const vimcube::geometry::Point3d& pointOnPlane);

    PlaneSide classifySignedDistance(const float d);

    vimcube::geometry::BoundingBox3d computeTriangleAABB(const vimcube::geometry::Point3d& p1, const vimcube::geometry::Point3d& p2, const vimcube::geometry::Point3d& p3);

    bool isAABBOverlap(const vimcube::geometry::BoundingBox3d& a, const vimcube::geometry::BoundingBox3d& b);

    TriangleIntersection classifyTriangleIntersection(const vimcube::geometry::Triangle& a, const vimcube::geometry::Triangle& b);

    PointRelation classifyPointRelation(const vimcube::geometry::Point3d& p, const vimcube::geometry::Mesh& m);
}
#endif
