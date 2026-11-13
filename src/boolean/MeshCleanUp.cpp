#include "MeshCleanUp.h"
#include <sys/types.h>
#include <cstdint>
#include <limits>
#include <vector>
#include <tuple>
#include <cmath>
#include <map>

namespace vimcube::boolean
{
    namespace
    {
        /*
            Function Name : convertPoint3dToCell
            Parameters : const vimcube::geometry::Point3d& pt
            Return Type : std::tuple<int, int, int>
            Description : Convert Point3d to cell
        */
        std::tuple<int, int, int> convertPoint3dToCell(const vimcube::geometry::Point3d& pt)
        {
            std::tuple<int, int, int> t;
            std::get<0>(t) = static_cast<int>(std::floor(pt.x / EPSILON));
            std::get<1>(t) = static_cast<int>(std::floor(pt.y / EPSILON));
            std::get<2>(t) = static_cast<int>(std::floor(pt.z / EPSILON));
            return t;
        }

        /*
            Function Name : findWeldTarget
            Parameters : const std::tuple<int, int, int>& cells, const vimcube::geometry::Point3d& pt, const std::map<std::tuple<int, int, int>, std::vector<uint32_t>>& cellMap, const std::vector<vimcube::geometry::Vertex>& newVertices
            Return Type : uint32_t
            Description : Find indices of vertices using EPSILON (return uint32_t max if not found)
        */
        uint32_t findWeldTarget(const std::tuple<int, int, int>& cells, const vimcube::geometry::Point3d& pt, const std::map<std::tuple<int, int, int>, std::vector<uint32_t>>& cellMap, const std::vector<vimcube::geometry::Vertex>& newVertices)
        {
            auto [cx, cy, cz] = cells;
            int dir[3] = {-1, 0, 1};

            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        auto testX = cx + dir[i];
                        auto testY = cy + dir[j];
                        auto testZ = cz + dir[k];

                        auto it = cellMap.find(std::make_tuple(testX, testY, testZ));
                        if (it != cellMap.end())
                        {
                            for (const auto& c : it->second)
                            {
                                float lengthSquare = (newVertices[c].position.x - pt.x) * (newVertices[c].position.x - pt.x) + (newVertices[c].position.y - pt.y) * (newVertices[c].position.y - pt.y) + (newVertices[c].position.z - pt.z) * (newVertices[c].position.z - pt.z);
                                if (lengthSquare <= EPSILON_SQUARE)
                                {
                                    return c;
                                }
                            }
                        }
                    }
                }
            }
            return std::numeric_limits<uint32_t>::max();
        }
    }

    /*
        Function Name : weldVertices
        Parameters : const vimcube::geometry::Mesh& m
        Return Type : WeldResult
        Description : Weld vertices in the same position
    */
    WeldResult weldVertices(const vimcube::geometry::Mesh& m)
    {
        WeldResult result;
        std::vector<vimcube::geometry::Vertex> newVertices;
        std::vector<uint32_t> remap(m.getVertices().size());

        std::map<std::tuple<int, int, int>, std::vector<uint32_t>> cellMap;
        size_t vertexIdx = 0;
        for (const auto& v : m.getVertices())
        {
            std::tuple<int, int, int> cellKeys = convertPoint3dToCell(v.position);
            
            uint32_t weldTargetIdx = findWeldTarget(cellKeys, v.position, cellMap, newVertices);
            // if not found
            if (weldTargetIdx == std::numeric_limits<uint32_t>::max())
            {
                newVertices.push_back(v);
                remap[vertexIdx] = static_cast<uint32_t>(newVertices.size() - 1);

                cellMap[cellKeys].push_back(static_cast<uint32_t>(newVertices.size() - 1));
            }
            else
            {
                remap[vertexIdx] = weldTargetIdx;
                result.mergedVerticesNum++;
            }
            vertexIdx++;
        }

        std::vector<vimcube::geometry::Face> newFaces;
        for (const auto& f : m.getFaces())
        {
            newFaces.push_back(vimcube::geometry::Face(remap[f.v0], remap[f.v1], remap[f.v2]));
        }

        vimcube::geometry::Mesh newMesh(newFaces, newVertices, std::vector<vimcube::geometry::Edge>());
        newMesh.setEdgesByFaces();
        result.mesh = std::move(newMesh);
        return result;
    }
}
