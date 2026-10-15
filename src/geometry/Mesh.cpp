#include "Mesh.h"
#include <cstdint>
#include <limits>
#include "GeoTypes.h"

namespace vimcube::geometry {
    
    /*
        Function Name : getFaces 
        Parameters : -
        Return Type : std::vector<Face>
        Description : Return faces
    */
    const std::vector<Face>& Mesh::getFaces() const
    {
        return faces_;
    }

    /*
        Function Name : getVertices 
        Parameters : -
        Return Type : std::vector<Vertex>
        Description : Return vertices
    */
    const std::vector<Vertex>& Mesh::getVertices() const
    {
        return vertices_;
    }
    
    /*
        Function Name : getEdges
        Parameters : -
        Return Type : std::vector<Edge>&
        Description : Return edges 
    */
    const std::vector<Edge>& Mesh::getEdges() const
    {
        return edges_;
    }

    /*
        Function Name : setEdgesByFaces
        Parameters : -
        Return Type : void
        Description : Set edges by given faces
    */
    void Mesh::setEdgesByFaces()
    {
        std::set<std::pair<uint32_t, uint32_t>> edgeList;

        for (auto f : this->faces_)
        {
            edgeList.insert(std::minmax(f.v0, f.v1));
            edgeList.insert(std::minmax(f.v1, f.v2));
            edgeList.insert(std::minmax(f.v2, f.v0));
        }

        this->edges_.clear();
        for (auto e : edgeList)
        {
            this->edges_.push_back(vimcube::geometry::Edge(e.first, e.second));
        }
    }

    /*
        Function Name : computeBoundingBox
        Parameters : -
        Return Type : vimcube::geometry::BoundingBox3d
        Description : Get bounding box of Mesh object
    */
    vimcube::geometry::BoundingBox3d Mesh::computeBoundingBox(const vimcube::math::Matrix4by4& modelMatrix) const
    {
        if (this->vertices_.empty()) return BoundingBox3d({0, 0, 0}, {0, 0, 0});

        vimcube::geometry::Point4d min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
        vimcube::geometry::Point4d max(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());

        for (const auto& v : this->vertices_)
        {
            vimcube::geometry::Point4d pt = vimcube::math::castToPoint4d(v.position);
            vimcube::geometry::Point4d worldPt = vimcube::math::getProduct4by4andPoint4d(modelMatrix, pt);

            min.x = std::min(min.x, worldPt.x);
            min.y = std::min(min.y, worldPt.y);
            min.z = std::min(min.z, worldPt.z);

            max.x = std::max(max.x, worldPt.x);
            max.y = std::max(max.y, worldPt.y);
            max.z = std::max(max.z, worldPt.z);
        }

        return BoundingBox3d({min.x, min.y, min.z}, {max.x, max.y, max.z});
    }
}
