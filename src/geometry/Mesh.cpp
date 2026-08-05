#include "Mesh.h"

namespace vimcube::geometry {
    /*
        Function Name : getVertices 
        Parameters : -
        Return Type : std::vector<Vertex>
        Description : Return vertices
    */
    const std::vector<Vertex> Mesh::getVertices() const
    {
        return vertices_;
    }
    
    /*
        Function Name : getEdges 
        Parameters : -
        Return Type : std::vector<Edge>
        Description : Return edges 
    */
    const std::vector<Edge> Mesh::getEdges() const
    {
        return edges_;
    }
}