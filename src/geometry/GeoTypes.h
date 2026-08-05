#ifndef GEO_TYPES_H
#define GEO_TYPES_H
#include <cstdint>
#include <vector>
#include "../render/Utils.h"

namespace vimcube::geometry {
    /*
        Class Name : Point3d
        Description : Point3d is constructed by three points.
    */
    struct Point3d {
        float x;
        float y;
        float z;

        Point3d() = default;
        Point3d(float x, float y, float z) : x(x), y(y), z(z) {};
    };

    /*
        Class Name : Point2d
        Description : Point2d is constructed by two points. This is used for 2D canvas drawing.
    */
    struct Point2d {
        float x;
        float y;

        Point2d() = default;
        Point2d(float x, float y) : x(x), y(y) {};
    };

    /*
        Class Name : PointCloud
        Description : PointCloud is a set of Point3d objects.
    */
    class PointCloud {
    private:
        std::vector<Point3d> ptCloud_;
    
    public:
        PointCloud() = default;
        PointCloud(std::vector<Point3d> ptCloud) : ptCloud_(ptCloud) {};
    };

    /*
        Class Name : Vector3d 
        Description : Vector3d is constructed by three points.
    */
    struct Vector3d {
        float x;
        float y;
        float z;

        Vector3d() = default;
        Vector3d(float x, float y, float z) : x(x), y(y), z(z) {};
    };

    /*
        Class Name : Line3d 
        Description : Line3d has two points: start and end.
    */
    struct Line3d {
        Point3d start_;
        Point3d end_;

        Line3d() = default;
        Line3d(Point3d start, Point3d end) : start_(start), end_(end) {};
    };

    /*
        Class Name : PolyLine 
        Description : PolyLine has multiple segments.
    */
    struct PolyLine {
        std::vector<Point3d> segments;

        PolyLine() = default;
        PolyLine(std::vector<Point3d> segments) : segments(segments) {};
    };

    /*
        Class Name : Plane 
        Description : Plane is constructed by normal vector and distance from origin point. (Ax + By + Cz + D = 0)
    */
    struct Plane {
        Vector3d normal;
        float distance;

        Plane() = default;
        Plane(Vector3d normal) : normal(normal) {};
    };

    /*
        Class Name : Vertex 
        Description : Vertex is simply a point in Mesh.
    */
    struct Vertex {
        Point3d position;
        Vector3d normal;
        vimcube::utility::Color color;

        Vertex(Point3d position) : position(position), normal(Vector3d(0.0f, 0.0f, 0.0f)), color(255, 255, 255, 1.0f) {};
        Vertex(Point3d position, Vector3d normal) : position(position), normal(normal), color(255, 255, 255, 1.0f) {};
        Vertex(Point3d position, Vector3d normal, vimcube::utility::Color color) : position(position), normal(normal), color(color) {};
    };

    /*
        Class Name : Edge
        Description : Edge objects are lines in Mesh.
    */
    struct Edge {
        uint32_t v0;
        uint32_t v1;

        Edge() = default;
        Edge(uint32_t v0, uint32_t v1) : v0(v0), v1(v1) {};
    };

    /*
        Class Name : Face 
        Description : Face objects construct a Mesh.
    */
    struct Face {
        uint32_t v0;    // Vertex index
        uint32_t v1;
        uint32_t v2;
    };
}
#endif