#include "GeoFactory.h"
#include <vector>

namespace vimcube::geo_factory {
    /*
        Function Name : createCube 
        Parameters : float size
        Return Type : vimcube::geometry::Mesh 
        Description : Create a Mesh object
    */
    vimcube::geometry::Mesh createCube(float size)
    {
        float half = size / 2.0f;
        std::vector<vimcube::geometry::Vertex> vertices;
        
        vimcube::geometry::Point3d pt0(half, half, half);
        vimcube::geometry::Point3d pt1(-half, half, half);
        vimcube::geometry::Point3d pt2(-half, -half, half);
        vimcube::geometry::Point3d pt3(half, -half, half);
        vimcube::geometry::Point3d pt4(half, half, -half);
        vimcube::geometry::Point3d pt5(-half, half, -half);
        vimcube::geometry::Point3d pt6(-half, -half, -half);
        vimcube::geometry::Point3d pt7(half, -half, -half);
        
        vertices.push_back(vimcube::geometry::Vertex(pt0)); // 0
        vertices.push_back(vimcube::geometry::Vertex(pt1)); // 1
        vertices.push_back(vimcube::geometry::Vertex(pt2)); // 2
        vertices.push_back(vimcube::geometry::Vertex(pt3)); // 3
        vertices.push_back(vimcube::geometry::Vertex(pt4)); // 4
        vertices.push_back(vimcube::geometry::Vertex(pt5)); // 5
        vertices.push_back(vimcube::geometry::Vertex(pt6)); // 6
        vertices.push_back(vimcube::geometry::Vertex(pt7)); // 7

        std::vector<vimcube::geometry::Edge> edges;

        vimcube::geometry::Edge e0(vimcube::geometry::Edge(0, 1));
        vimcube::geometry::Edge e1(vimcube::geometry::Edge(1, 2));
        vimcube::geometry::Edge e2(vimcube::geometry::Edge(2, 3));
        vimcube::geometry::Edge e3(vimcube::geometry::Edge(3, 0));
        vimcube::geometry::Edge e4(vimcube::geometry::Edge(4, 5));
        vimcube::geometry::Edge e5(vimcube::geometry::Edge(5, 6));
        vimcube::geometry::Edge e6(vimcube::geometry::Edge(6, 7));
        vimcube::geometry::Edge e7(vimcube::geometry::Edge(7, 4));
        vimcube::geometry::Edge e8(vimcube::geometry::Edge(0, 4));
        vimcube::geometry::Edge e9(vimcube::geometry::Edge(1, 5));
        vimcube::geometry::Edge e10(vimcube::geometry::Edge(2, 6));
        vimcube::geometry::Edge e11(vimcube::geometry::Edge(3, 7));

        edges.push_back(e0);
        edges.push_back(e1);
        edges.push_back(e2);
        edges.push_back(e3);
        edges.push_back(e4);
        edges.push_back(e5);
        edges.push_back(e6);
        edges.push_back(e7);
        edges.push_back(e8);
        edges.push_back(e9);
        edges.push_back(e10);
        edges.push_back(e11);

        return vimcube::geometry::Mesh(vertices, edges);
    }
}