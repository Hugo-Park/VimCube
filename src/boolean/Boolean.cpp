#include "Boolean.h"
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
}
