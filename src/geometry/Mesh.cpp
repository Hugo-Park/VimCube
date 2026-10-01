#include "Mesh.h"
#include <cstdint>
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
}
