#include "WorldMesh.h"
#include <vector>
#include <cstdint>
#include <utility>

namespace vimcube::boolean
{
    /*
        Function Name : convertToWorldMesh
        Parameters : const vimcube::geometry::Mesh& m, const vimcube::geometry::Transform& t
        Return Type : WorldMeshResult
        Description : Convert normal mesh to new mesh with world vertices
    */
    WorldMeshResult convertToWorldMesh(const vimcube::geometry::Mesh& m, const vimcube::geometry::Transform& t)
    {
        WorldMeshResult result;

        vimcube::math::Matrix4by4 modelMat = t.getModelMatrix();
        vimcube::math::Matrix3by3 partialMat = vimcube::math::getPartialFrom4by4(modelMat);
        float det = vimcube::math::getDet3by3(partialMat);
        if (det == 0)
        {
            result.errorMessage = "Invalid model matrix";
            return result;
        }
        vimcube::math::Matrix3by3 inverseTranspose = vimcube::math::getTranspose3by3(vimcube::math::getInverse3by3(partialMat));

        std::vector<vimcube::geometry::Vertex> newVertices;
        for (const auto& v : m.getVertices())
        {
            // Convert its position
            vimcube::geometry::Point4d pt = math::castToPoint4d(v.position);
            vimcube::geometry::Point4d pt4d = vimcube::math::getProduct4by4andPoint4d(modelMat, pt);

            vimcube::geometry::Vector3d vec = vimcube::math::getProduct3by3andVector3d(inverseTranspose, v.normal);
            if (v.normal.getSize() == 0)
            {
                vimcube::geometry::Vertex newVertex(vimcube::math::castToPoint3d(pt4d), vec);
                newVertices.push_back(newVertex);
            }
            else
            {
                vimcube::geometry::Vertex newVertex(vimcube::math::castToPoint3d(pt4d), vimcube::math::normalizeVector3d(vec));
                newVertices.push_back(newVertex);
            }
        }

        std::vector<vimcube::geometry::Face> newFaces;
        for (auto f : m.getFaces())
        {
            if (det < 0)
            {
                uint32_t temp = f.v1;
                f.v1 = f.v2;
                f.v2 = temp;
            }
            newFaces.push_back(f);
        }

        vimcube::geometry::Mesh newMesh(newFaces, newVertices, std::vector<vimcube::geometry::Edge>());
        newMesh.setEdgesByFaces();
        result.mesh = std::move(newMesh);

        return result;
    }
}
