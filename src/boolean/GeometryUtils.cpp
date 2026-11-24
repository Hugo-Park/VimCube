#include "GeometryUtils.h"
#include "Tolerance.h"
#include "../utility/Math.h"
#include <algorithm>

namespace vimcube::boolean
{
    /*
        Function Name : getTriangleInfoFromPoints
        Parameters : const vimcube::geometry::Point3d& p1, const vimcube::geometry::Point3d& p2, const vimcube::geometry::Point3d& p3
        Return Type : TriangleInfo
        Description : Get information of a triangle from three points
    */
    TriangleInfo getTriangleInfoFromPoints(const vimcube::geometry::Point3d& p1, const vimcube::geometry::Point3d& p2, const vimcube::geometry::Point3d& p3)
    {
        TriangleInfo result;

        // n = (b - a) x (c - a)
        vimcube::geometry::Vector3d vec1 = { p2.x - p1.x, p2.y - p1.y, p2.z - p1.z };
        vimcube::geometry::Vector3d vec2 = { p3.x - p1.x, p3.y - p1.y, p3.z - p1.z };

        vimcube::geometry::Vector3d product = vimcube::math::crossProductVector3d(vec1, vec2);

        float d1 = vimcube::math::getDistance(p1, p2);
        float d2 = vimcube::math::getDistance(p2, p3);
        float maxDist = std::max(d1, d2);
        float d3 = vimcube::math::getDistance(p3, p1);
        maxDist = std::max(maxDist, d3);

        if (product.getSize() <= maxDist * maxDist * EPSILON_DEGEN_TRIANGLE_RATIO)
        {
            return result;
        }
        result.normal = vimcube::math::normalizeVector3d(product);
        result.area = product.getSize() / 2.0f;

        result.isDegenerate = false;
        return result;
    }

    /*
        Function Name : computeSignedDistance
        Parameters : const vimcube::geometry::Vector3d& normal, const vimcube::geometry::Point3d& point, const vimcube::geometry::Point3d& pointOnPlane
        Return Type : float
        Description : Compute signed distance between a point and plane
    */
    float computeSignedDistance(const vimcube::geometry::Vector3d& normal, const vimcube::geometry::Point3d& point, const vimcube::geometry::Point3d& pointOnPlane)
    {
        // d = n * (p - a)
        return vimcube::math::dotProductVector3d(normal, vimcube::math::subtractVector3d(vimcube::math::castToVector3d(point), vimcube::math::castToVector3d(pointOnPlane)));
    }

    /*
        Function Name : classifySignedDistance
        Parameters : const float d
        Return Type : PlaneSide
        Description : Classify the location of the point related to plane by signed distance(should check isDegenerate before calling method / *if d is NaN, it returns ON_PLANE*)
    */
    PlaneSide classifySignedDistance(const float d)
    {
        if (d > EPSILON_PLANE_DISTANCE) return PlaneSide::FORWARD;
        else if (d < -EPSILON_PLANE_DISTANCE) return PlaneSide::BACKWARD;
        return PlaneSide::ON_PLANE;
    }

    /*
        Function Name : computeTriangleAABB
        Parameters : const vimcube::geometry::Point3d& p1, const vimcube::geometry::Point3d& p2, const vimcube::geometry::Point3d& p3
        Return Type : vimcube::geometry::BoundingBox3d
        Description : Get bounding box of a single triangle with three points
    */
    vimcube::geometry::BoundingBox3d computeTriangleAABB(const vimcube::geometry::Point3d& p1, const vimcube::geometry::Point3d& p2, const vimcube::geometry::Point3d& p3)
    {
        vimcube::geometry::BoundingBox3d bbox;

        bbox.min.x = std::min(p1.x, p2.x);
        bbox.min.x = std::min(bbox.min.x, p3.x);

        bbox.max.x = std::max(p1.x, p2.x);
        bbox.max.x = std::max(bbox.max.x, p3.x);

        bbox.min.y = std::min(p1.y, p2.y);
        bbox.min.y = std::min(bbox.min.y, p3.y);

        bbox.max.y = std::max(p1.y, p2.y);
        bbox.max.y = std::max(bbox.max.y, p3.y);

        bbox.min.z = std::min(p1.z, p2.z);
        bbox.min.z = std::min(bbox.min.z, p3.z);

        bbox.max.z = std::max(p1.z, p2.z);
        bbox.max.z = std::max(bbox.max.z, p3.z);

        return bbox;
    }

    /*
        Function Name : isAABBOverlap
        Parameters : const vimcube::geometry::BoundingBox3d& a, const vimcube::geometry::BoundingBox3d& b
        Return Type : bool
        Description : Check overlapping of two bounding boxes
    */
    bool isAABBOverlap(const vimcube::geometry::BoundingBox3d& a, const vimcube::geometry::BoundingBox3d& b)
    {
        if (a.max.x + EPSILON_AABB_MARGIN < b.min.x || b.max.x + EPSILON_AABB_MARGIN < a.min.x) return false;
        if (a.max.y + EPSILON_AABB_MARGIN < b.min.y || b.max.y + EPSILON_AABB_MARGIN < a.min.y) return false;
        if (a.max.z + EPSILON_AABB_MARGIN < b.min.z || b.max.z + EPSILON_AABB_MARGIN < a.min.z) return false;
        return true;
    }
}
