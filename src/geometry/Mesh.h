#ifndef MESH_H
#define MESH_H
#include <vector>
#include "GeoTypes.h"

namespace vimcube::geometry {
    /*
        Class Name : Mesh 
        Description : Mesh is a basic 3D object type in VimCube.
    */
    class Mesh {
    private:
        std::vector<Vertex> vertices_;
        std::vector<Edge> edges_;  // Vertex connections with Line3d
    
    public:
        Mesh() = default;
        Mesh(const std::vector<Vertex>& vertices, const std::vector<Edge>& edges) : vertices_(vertices), edges_(edges) {};
        const std::vector<Vertex>& getVertices() const;
        const std::vector<Edge>& getEdges() const;
    };
}
#endif