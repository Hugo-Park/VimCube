#ifndef MESH_H
#define MESH_H
#include <vector>
#include <cstdint>
#include <set>
#include  <algorithm>
#include "GeoTypes.h"

namespace vimcube::geometry {
    /*
        Class Name : Mesh 
        Description : Mesh is a basic 3D object type in VimCube.
    */
    class Mesh {
    private:
        std::vector<Face> faces_;
        std::vector<Vertex> vertices_;
        std::vector<Edge> edges_;  // Vertex connections with Line3d
    
    public:
        Mesh() = default;
        Mesh(const std::vector<Vertex>& vertices, const std::vector<Edge>& edges) : vertices_(vertices), edges_(edges) {};
        Mesh(const std::vector<Face>& faces, const std::vector<Vertex>& vertices, const std::vector<Edge>& edges) : faces_(faces), vertices_(vertices), edges_(edges) {};
        const std::vector<Face>& getFaces() const;
        const std::vector<Vertex>& getVertices() const;
        const std::vector<Edge>& getEdges() const;

        void setEdgesByFaces();
    };
}
#endif
