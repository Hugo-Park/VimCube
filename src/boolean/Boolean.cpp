#include "Boolean.h"
#include <sys/types.h>
#include <limits>
#include <string>
#include <algorithm>
#include <cstdint>
#include <map>
#include <utility>

namespace vimcube::boolean
{
    namespace
    {
        /*
            Function Name : extractEdgeFromMesh
            Parameters : const vimcube::geometry::Mesh& m
            Return Type : std::map<std::pair<uint32_t, uint32_t>, int>
            Description : Extract edge index map from Mesh
        */
        std::map<std::pair<uint32_t, uint32_t>, int> extractEdgeFromMesh(const vimcube::geometry::Mesh& m)
        {
            std::map<std::pair<uint32_t, uint32_t>, int> result;

            for (const auto & f : m.getFaces())
            {
                std::pair<uint32_t, uint32_t> p1;
                std::pair<uint32_t, uint32_t> p2;
                std::pair<uint32_t, uint32_t> p3;
                p1 = std::minmax(f.v0, f.v1);
                p2 = std::minmax(f.v1, f.v2);
                p3 = std::minmax(f.v0, f.v2);
                
                result[p1]++;
                result[p2]++;
                result[p3]++;
            }

            return result;
        }
        
        std::map<std::pair<uint32_t, uint32_t>, int> extractDirectedEdgeFromMesh(const vimcube::geometry::Mesh& m)
        {
            std::map<std::pair<uint32_t, uint32_t>, int> result;

            for (const auto & f : m.getFaces())
            {
                std::pair<uint32_t, uint32_t> p1;
                std::pair<uint32_t, uint32_t> p2;
                std::pair<uint32_t, uint32_t> p3;
                p1 = {f.v0, f.v1};
                p2 = {f.v1, f.v2};
                p3 = {f.v2, f.v0};
                
                result[p1]++;
                result[p2]++;
                result[p3]++;
            }

            return result;
        }
    }

    /*
        Function Name : checkIsMeshClosed
        Parameters : const vimcube::geometry::Mesh& m
        Return Type : WaterTightResult
        Description : Check if mesh is closed
    */
    WaterTightResult checkIsMeshClosed(const vimcube::geometry::Mesh& m)
    {
        WaterTightResult result;

        size_t verticesNum = m.getVertices().size();
        for (const auto& f : m.getFaces())
        {
            if (f.v0 >= verticesNum || f.v1 >= verticesNum || f.v2 >= verticesNum)
            {
                result.errorMessage = "Out of index range";
                return result;
            }

            if (f.v0 == f.v1 || f.v1 == f.v2 || f.v0 == f.v2)
            {
                result.errorMessage = "Degenerate triangles are detected";
                return result;
            }
        }

        std::map<std::pair<uint32_t, uint32_t>, int> edgeMap = extractEdgeFromMesh(m);
        for (const auto& [edge, num] : edgeMap)
        {
            if (num == 1)
            {
                result.openEdgeNum++;
            }
            else if (num >= 3)
            {
                result.nonManifoldEdgeNum++;
            }
        }

        if (result.openEdgeNum || result.nonManifoldEdgeNum)
        {
            result.errorMessage = std::to_string(result.openEdgeNum) + " edges are not closed, " + std::to_string(result.nonManifoldEdgeNum) + " edges are non-manifold";
            return result;
        }

        result.isClosed = true;
        return result;
    }

    /*
        Function Name : checkIsWindingConsistent
        Parameters : const vimcube::geometry::Mesh& m
        Return Type : WindingResult
        Description : Check if edges are inconsistent
    */
    WindingResult checkIsWindingConsistent(const vimcube::geometry::Mesh& m)
    {
        WindingResult result;

        size_t verticesNum = m.getVertices().size();
        for (const auto& f : m.getFaces())
        {
            if (f.v0 >= verticesNum || f.v1 >= verticesNum || f.v2 >= verticesNum)
            {
                result.errorMessage = "Out of index range";
                return result;
            }

            if (f.v0 == f.v1 || f.v1 == f.v2 || f.v0 == f.v2)
            {
                result.errorMessage = "Degenerate triangles are detected";
                return result;
            }
        }

        std::map<std::pair<uint32_t, uint32_t>, int> edgeMap = extractDirectedEdgeFromMesh(m);
        for (const auto& [edge, num] : edgeMap)
        {
            if (num >= 2)
            {
                result.inconsistentNum++;
            }
        }

        if (result.inconsistentNum)
        {
            result.errorMessage = std::to_string(result.inconsistentNum) + " edges are inconsistent";
            return result;
        }

        result.isConsistent = true;
        return result;
    }

    /*
        Function Name : computeSignedVolume
        Parameters : const vimcube::geometry::Mesh& m
        Return Type : float
        Description : Compute volume of mesh with signed (return NaN if error occurs)
    */
    float computeSignedVolume(const vimcube::geometry::Mesh& m)
    {
        double sumVolume = 0;

        size_t verticesNum = m.getVertices().size();
        for (const auto& f : m.getFaces())
        {
            if (f.v0 >= verticesNum || f.v1 >= verticesNum || f.v2 >= verticesNum)
            {
                return std::numeric_limits<float>::quiet_NaN();
            }

            if (f.v0 == f.v1 || f.v1 == f.v2 || f.v0 == f.v2)
            {
                return std::numeric_limits<float>::quiet_NaN();
            }

            vimcube::geometry::Vector3d vec0 = vimcube::math::castToVector3d(m.getVertices()[f.v0].position);
            vimcube::geometry::Vector3d vec1 = vimcube::math::castToVector3d(m.getVertices()[f.v1].position);
            vimcube::geometry::Vector3d vec2 = vimcube::math::castToVector3d(m.getVertices()[f.v2].position);

            sumVolume += static_cast<double>(vimcube::math::dotProductVector3d(vec0, vimcube::math::crossProductVector3d(vec1, vec2)));
        }

        return static_cast<float>(sumVolume / 6.0);
    }
}
