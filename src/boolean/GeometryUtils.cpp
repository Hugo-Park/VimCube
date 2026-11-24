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
}
