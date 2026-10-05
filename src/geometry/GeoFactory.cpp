#include "GeoFactory.h"
#include <cstdint>
#include <vector>
#include "GeoTypes.h"
#include "Mesh.h"

namespace vimcube::geo_factory {
    /*
        Function Name : createAxisIndicator
        Parameters : float length
        Return Type : vimcube::geometry::Mesh 
        Description : Create an axis indicator
    */
    vimcube::geometry::Mesh createAxisIndicator(float length)
    {
        std::vector<vimcube::geometry::Vertex> vertices;

        vimcube::geometry::Point3d pt0(0, 0, 0);
        vimcube::geometry::Point3d pt1(length, 0, 0);
        vimcube::geometry::Point3d pt2(0, length, 0);
        vimcube::geometry::Point3d pt3(0, 0, length);

        vertices.push_back(vimcube::geometry::Vertex(pt0));
        vertices.push_back(vimcube::geometry::Vertex(pt1));
        vertices.push_back(vimcube::geometry::Vertex(pt2));
        vertices.push_back(vimcube::geometry::Vertex(pt3));

        std::vector<vimcube::geometry::Edge> edges;

        vimcube::geometry::Edge e0(vimcube::geometry::Edge(0, 1));
        vimcube::geometry::Edge e1(vimcube::geometry::Edge(0, 2));
        vimcube::geometry::Edge e2(vimcube::geometry::Edge(0, 3));

        edges.push_back(e0);
        edges.push_back(e1);
        edges.push_back(e2);

        return vimcube::geometry::Mesh(vertices, edges);
    }

    /*
        Function Name : createCube 
        Parameters : float size
        Return Type : vimcube::geometry::Mesh 
        Description : Create a cube mesh
    */
    vimcube::geometry::Mesh createCube(float size)
    {
        float half = size / 2.0f;
        std::vector<vimcube::geometry::Face> faces;
        std::vector<vimcube::geometry::Vertex> vertices;
        std::vector<vimcube::geometry::Edge> edges;
        
        vimcube::geometry::Point3d pt0(half, half, half);
        vimcube::geometry::Point3d pt1(half, -half, half);
        vimcube::geometry::Point3d pt2(half, -half, -half);
        vimcube::geometry::Point3d pt3(half, half, -half);
        vimcube::geometry::Point3d pt4(-half, half, half);
        vimcube::geometry::Point3d pt5(-half, -half, half);
        vimcube::geometry::Point3d pt6(-half, -half, -half);
        vimcube::geometry::Point3d pt7(-half, half, -half);

        vertices.push_back(vimcube::geometry::Vertex(pt0)); // 0
        vertices.push_back(vimcube::geometry::Vertex(pt1)); // 1
        vertices.push_back(vimcube::geometry::Vertex(pt2)); // 2
        vertices.push_back(vimcube::geometry::Vertex(pt3)); // 3
        vertices.push_back(vimcube::geometry::Vertex(pt4)); // 4
        vertices.push_back(vimcube::geometry::Vertex(pt5)); // 5
        vertices.push_back(vimcube::geometry::Vertex(pt6)); // 6
        vertices.push_back(vimcube::geometry::Vertex(pt7)); // 7

        vimcube::geometry::Face f0(0, 1, 2);
        vimcube::geometry::Face f1(0, 2, 3);
        vimcube::geometry::Face f2(4, 0, 3);
        vimcube::geometry::Face f3(4, 3, 7);
        vimcube::geometry::Face f4(5, 4, 7);
        vimcube::geometry::Face f5(5, 7, 6);
        vimcube::geometry::Face f6(1, 5, 6);
        vimcube::geometry::Face f7(1, 6, 2);
        vimcube::geometry::Face f8(0, 4 ,5);
        vimcube::geometry::Face f9(0, 5, 1);
        vimcube::geometry::Face f10(3, 2, 6);
        vimcube::geometry::Face f11(3, 6, 7);

        faces.push_back(f0);
        faces.push_back(f1);
        faces.push_back(f2);
        faces.push_back(f3);
        faces.push_back(f4);
        faces.push_back(f5);
        faces.push_back(f6);
        faces.push_back(f7);
        faces.push_back(f8);
        faces.push_back(f9);
        faces.push_back(f10);
        faces.push_back(f11);

        vimcube::geometry::Mesh cubeMesh(faces, vertices, edges);
        cubeMesh.setEdgesByFaces();
        return cubeMesh;
    }

    vimcube::geometry::Mesh createCubeOld(float size)
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

    /*
        Function Name : createSphere 
        Parameters : float radius, int rings, int segments
        Return Type : vimcube::geometry::Mesh 
        Description : Create a mesh sphere
    */
    vimcube::geometry::Mesh createSphere(float radius, int rings, int segments)
    {
        std::vector<vimcube::geometry::Face> faces;
        std::vector<vimcube::geometry::Vertex> vertices;
        std::vector<vimcube::geometry::Edge> edges;
        for (int i = 0; i <= rings; i++)
        {
            for (int j = 0; j < segments; j++)
            {
                float theta = i * PI / rings;
                float phi = j * 2 * PI / segments;

                float x = radius * std::sin(theta) * std::cos(phi);
                float y = radius * std::sin(theta) * std::sin(phi);
                float z = radius * std::cos(theta);
                vertices.push_back(vimcube::geometry::Point3d(x, y, z));
            }
        }

        for (int i = 0; i < rings; i++)
        {
            for (int j = 0; j < segments; j++)
            {
                uint32_t v00 = i * segments + j;
                uint32_t v01 = i * segments + (j + 1) % segments;
                uint32_t v10 = (i + 1) * segments + j;
                uint32_t v11 = (i + 1) * segments + (j + 1) % segments;

                vimcube::geometry::Face f0(v00, v11, v01);
                vimcube::geometry::Face f1(v00, v10, v11);
                faces.push_back(f0);
                faces.push_back(f1);
            }
        }
        vimcube::geometry::Mesh sphereMesh(faces, vertices, edges);
        sphereMesh.setEdgesByFaces();

        return sphereMesh;
    }
}
