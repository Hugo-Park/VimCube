#include "GeometryUtils.h"
#include "Tolerance.h"
#include "../utility/Math.h"
#include <algorithm>
#include <array>
#include <optional>
#include <utility>

namespace vimcube::boolean
{
    namespace
    {
        /*
            Function Name : computeInterpolatePoint
            Parameters : const vimcube::geometry::Point3d& pi, const vimcube::geometry::Point3d& pj, float di, float dj
            Return Type : vimcube::geometry::Point3d
            Description : Compute an interpolation point between two points
        */
        vimcube::geometry::Point3d computeInterpolatePoint(const vimcube::geometry::Point3d& pi, const vimcube::geometry::Point3d& pj, float di, float dj)
        {
            float m = di / (di - dj);
            vimcube::geometry::Point3d p((pj.x - pi.x) * m, (pj.y - pi.y) * m, (pj.z - pi.z) * m);

            return vimcube::geometry::Point3d(pi.x + p.x, pi.y + p.y, pi.z + p.z);
        }

        /*
            Function Name : computePlaneCrossingPoints
            Parameters : const vimcube::geometry::Triangle& t, const std::array<float, 3>& dists, const std::array<PlaneSide, 3>& testCase
            Return Type : std::optional<std::array<vimcube::geometry::Point3d, 2>>
            Description : Get two intersection points
        */
        std::optional<std::array<vimcube::geometry::Point3d, 2>> computePlaneCrossingPoints(const vimcube::geometry::Triangle& t, const std::array<float, 3>& dists, const std::array<PlaneSide, 3>& testCase)
        {
            std::array<vimcube::geometry::Point3d, 2> intersections;
            std::array<vimcube::geometry::Point3d, 3> vertices = {t.p0, t.p1, t.p2};

            size_t idx = 0;
            for (size_t i = 0; i < 3; i++)
            {
                size_t j = (i + 1) % 3;
                if(testCase.at(i) == PlaneSide::ON_PLANE)
                {
                    intersections.at(idx) = vertices.at(i);
                    idx++;
                }
                else if (testCase.at(i) == PlaneSide::FORWARD && testCase.at(j) == PlaneSide::BACKWARD)
                {
                    intersections.at(idx) = computeInterpolatePoint(vertices.at(i), vertices.at(j), dists.at(i), dists.at(j));
                    idx++;
                }
                else if (testCase.at(i) == PlaneSide::BACKWARD && testCase.at(j) == PlaneSide::FORWARD)
                {
                    intersections.at(idx) = computeInterpolatePoint(vertices.at(i), vertices.at(j), dists.at(i), dists.at(j));
                    idx++;
                }
            }

            if (idx < 2) return std::nullopt;

            return intersections;
        }
    }

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

    /*
        Function Name : classifyTriangleIntersection
        Parameters : const vimcube::geometry::Triangle& a, const vimcube::geometry::Triangle& b
        Return Type : TriangleIntersection
        Description : Check intersection of two triangles
    */
    TriangleIntersection classifyTriangleIntersection(const vimcube::geometry::Triangle& a, const vimcube::geometry::Triangle& b)
    {
        TriangleIntersection result;

        vimcube::geometry::BoundingBox3d b1 = computeTriangleAABB(a.p0, a.p1, a.p2);
        vimcube::geometry::BoundingBox3d b2 = computeTriangleAABB(b.p0, b.p1, b.p2);

        if (!isAABBOverlap(b1, b2))
        {
            return result;
        }

        TriangleInfo t1 = getTriangleInfoFromPoints(a.p0, a.p1, a.p2);
        TriangleInfo t2 = getTriangleInfoFromPoints(b.p0, b.p1, b.p2);

        if (t1.isDegenerate || t2.isDegenerate)
        {
            result.relation = TriangleRelation::INVALID;
            return result;
        }

        // compute signed distances from t1 to t2 and classify them
        std::array<PlaneSide, 3> testCaseA;
        std::array<PlaneSide, 3> testCaseB;

        std::array<float, 3> distA;
        std::array<float, 3> distB;

        size_t iA = 0;
        for (const auto& pt : std::array{b.p0, b.p1, b.p2})
        {
            distA.at(iA) = computeSignedDistance(t1.normal, pt, a.p0);
            testCaseA.at(iA) = classifySignedDistance(distA.at(iA));
            iA++;
        }

        size_t iB = 0;
        for (const auto& pt : std::array{a.p0, a.p1, a.p2})
        {
            distB.at(iB) = computeSignedDistance(t2.normal, pt, b.p0);
            testCaseB.at(iB) = classifySignedDistance(distB.at(iB));
            iB++;
        }

        // test if two triangles intersect
        for (const auto& t : std::array{testCaseA, testCaseB})
        {
            if (t.at(0) == PlaneSide::ON_PLANE && t.at(1) == PlaneSide::ON_PLANE && t.at(2) == PlaneSide::ON_PLANE)
            {
                result.relation = TriangleRelation::COPLANAR;
                return result;
            }
            if (t.at(0) != PlaneSide::FORWARD && t.at(1) != PlaneSide::FORWARD && t.at(2) != PlaneSide::FORWARD)
            {
                return result;
            }
            if (t.at(0) != PlaneSide::BACKWARD && t.at(1) != PlaneSide::BACKWARD && t.at(2) != PlaneSide::BACKWARD)
            {
                return result;
            }
        }

        // get intersect points
        std::optional<std::array<vimcube::geometry::Point3d, 2>> intersectA = computePlaneCrossingPoints(a, distB, testCaseB);
        std::optional<std::array<vimcube::geometry::Point3d, 2>> intersectB = computePlaneCrossingPoints(b, distA, testCaseA);
        if (!intersectA || !intersectB) return result;

        // direction of an intersect line
        vimcube::geometry::Vector3d dir = vimcube::math::crossProductVector3d(t1.normal, t2.normal);
        if (dir.getSize() <= EPSILON_COPLANAR_TEST)
        {
            result.relation = TriangleRelation::COPLANAR;
            return result;
        }

        vimcube::geometry::Vector3d nDir = vimcube::math::normalizeVector3d(dir);

        std::array<std::pair<vimcube::geometry::Point3d, float>, 2> pairInfoA;
        std::array<std::pair<vimcube::geometry::Point3d, float>, 2> pairInfoB;

        size_t i = 0;
        for (const auto& p : *intersectA)
        {
            pairInfoA.at(i).first = p;
            pairInfoA.at(i).second = vimcube::math::dotProductVector3d(nDir, vimcube::math::castToVector3d(p));
            i++;
        }

        i = 0;
        for (const auto& p : *intersectB)
        {
            pairInfoB.at(i).first = p;
            pairInfoB.at(i).second = vimcube::math::dotProductVector3d(nDir, vimcube::math::castToVector3d(p));
            i++;
        }

        // correct order (low -> high)
        if (pairInfoA.at(0).second > pairInfoA.at(1).second)
        {
            std::swap(pairInfoA.at(0), pairInfoA.at(1));
        }
        if (pairInfoB.at(0).second > pairInfoB.at(1).second)
        {
            std::swap(pairInfoB.at(0), pairInfoB.at(1));
        }

        // extract overlapping interval
        auto start = std::max(pairInfoA.at(0), pairInfoB.at(0), [](const auto& l, const auto& r) {
            return l.second < r.second;
        });
        auto end = std::min(pairInfoA.at(1), pairInfoB.at(1), [](const auto& l, const auto& r) {
            return l.second < r.second;
        });

        if (end.second - start.second <= EPSILON_PLANE_DISTANCE) return result;

        // finalize result
        result.relation = TriangleRelation::SEGMENT;
        result.start = start.first;
        result.end = end.first;
        return result;
    }
}
