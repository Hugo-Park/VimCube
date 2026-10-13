#include "GeoDraw.h"
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <vector>
#include "Camera.h"

namespace vimcube::geo_draw {
    namespace {

        /*
            Function Name : checkBoundary
            Parameters : float p, float q, float& t0, float& t1
            Return Type : bool
            Description : Check boundary for clipCanvas()
        */
        bool checkBoundary(float p, float q, float& t0, float& t1)
        {
            if (p == 0)
            {
                if (q < 0)
                    return false;
                return true;
            }

            float r = q / p;

            if (p < 0)
            {
                if (r > t1)
                    return false;
                else if (r > t0)
                    t0 = r;
            }
            else if (p > 0)
            {
                if (r < t0)
                    return false;
                else if (r < t1)
                    t1 = r;
            }
            return true;
        }

        /*
            Function Name : clipCanvas
            Parameters : vimcube::geometry::Point2d& pt1, vimcube::geometry::Point2d& pt2, ftxui::Canvas& canvas
            Return Type : bool
            Description : Check if geometry is out of screen
        */
        bool clipCanvas(vimcube::geometry::Point2d& pt1, vimcube::geometry::Point2d& pt2, ftxui::Canvas& canvas)
        {
            int xmin = 0;
            int ymin = 0;
            int xmax = canvas.width() - 1;
            int ymax = canvas.height() - 1;

            float dx = pt2.x - pt1.x;
            float dy = pt2.y - pt1.y;

            vimcube::geometry::Point2d o1 = pt1;

            float t0 = 0.0f;
            float t1 = 1.0f;

            bool check = checkBoundary(-dx, pt1.x - xmin, t0, t1) && checkBoundary(dx, xmax - pt1.x, t0, t1) && checkBoundary(-dy, pt1.y - ymin, t0, t1) && checkBoundary(dy, ymax - pt1.y, t0, t1);

            if (!check) return false;

            pt1.x = o1.x + t0 * dx;
            pt1.y = o1.y + t0 * dy;
            pt2.x = o1.x + t1 * dx;
            pt2.y = o1.y + t1 * dy;
            return true;
        }
    }

    /*
        Function Name : draw 
        Parameters : const vimcube::math::Matrix4by4& modelMatrix, vimcube::geometry::Mesh& mesh, ftxui::Canvas& canvas, ftxui::Color color
        Return Type : void
        Description : Draw a Mesh object
    */
    void draw(const vimcube::math::Matrix4by4& modelMatrix, vimcube::camera::Camera& camera, const vimcube::geometry::Mesh& mesh, ftxui::Canvas& canvas, ftxui::Color color)
    {

        for (const auto& edge : mesh.getEdges())
        {
            // Get real coordinates of vertices
            auto pt1 = mesh.getVertices()[edge.v0].position;
            auto pt2 = mesh.getVertices()[edge.v1].position;

            vimcube::geometry::Point2d point1 = camera.projectToCanvas(modelMatrix, pt1, canvas.width(), canvas.height());
            vimcube::geometry::Point2d point2 = camera.projectToCanvas(modelMatrix, pt2, canvas.width(), canvas.height());

            if(!clipCanvas(point1, point2, canvas))
                continue;
            int screenX1 = static_cast<int>(point1.x);
            int screenY1 = static_cast<int>(point1.y);
            int screenX2 = static_cast<int>(point2.x);
            int screenY2 = static_cast<int>(point2.y);
            canvas.DrawPointLine(screenX1, screenY1, screenX2, screenY2, color);
        }
    }

    /*
        Function Name : draw 
        Parameters : const vimcube::math::Matrix4by4& modelMatrix, vimcube::geometry::Line3d& line, ftxui::Canvas& canvas, ftxui::Color color
        Return Type : void
        Description : Draw a Line3d object
    */
    void draw(const vimcube::math::Matrix4by4& modelMatrix, vimcube::camera::Camera& camera, const vimcube::geometry::Line3d& line, ftxui::Canvas& canvas, ftxui::Color color)
    {
        auto pt1 = line.start_;
        auto pt2 = line.end_;

        vimcube::geometry::Point2d point1 = camera.projectToCanvas(modelMatrix, pt1, canvas.width(), canvas.height());
        vimcube::geometry::Point2d point2 = camera.projectToCanvas(modelMatrix, pt2, canvas.width(), canvas.height());

        if(!clipCanvas(point1, point2, canvas))
            return;

        int screenX1 = static_cast<int>(point1.x);
        int screenY1 = static_cast<int>(point1.y);
        int screenX2 = static_cast<int>(point2.x);
        int screenY2 = static_cast<int>(point2.y);
        canvas.DrawPointLine(screenX1, screenY1, screenX2, screenY2, color);
    }

    /*
        Function Name : draw 
        Parameters : const vimcube::math::Matrix4by4& modelMatrix, std::vector<vimcube::geometry::Line3d>& lines, ftxui::Canvas& canvas, ftxui::Color color
        Return Type : void
        Description : Draw Line3d objects
    */
    void draw(const vimcube::math::Matrix4by4& modelMatrix, vimcube::camera::Camera& camera, const std::vector<vimcube::geometry::Line3d>& lines, ftxui::Canvas& canvas, ftxui::Color color)
    {
        for (const auto& l : lines)
        {
            auto pt1 = l.start_;
            auto pt2 = l.end_;

            vimcube::geometry::Point2d point1 = camera.projectToCanvas(modelMatrix, pt1, canvas.width(), canvas.height());
            vimcube::geometry::Point2d point2 = camera.projectToCanvas(modelMatrix, pt2, canvas.width(), canvas.height());

            if(!clipCanvas(point1, point2, canvas))
                continue;

            int screenX1 = static_cast<int>(point1.x);
            int screenY1 = static_cast<int>(point1.y);
            int screenX2 = static_cast<int>(point2.x);
            int screenY2 = static_cast<int>(point2.y);
            canvas.DrawPointLine(screenX1, screenY1, screenX2, screenY2, color);
        }
    }

    /*
        Function Name : draw 
        Parameters : const vimcube::math::Matrix4by4& modelMatrix, std::vector<vimcube::geometry::Line3d>& lines, ftxui::Canvas& canvas, std::vector<ftxui::Color> colors
        Return Type : void
        Description : Draw Line3d objects with given colors in order
    */
    void draw(const vimcube::math::Matrix4by4& modelMatrix, vimcube::camera::Camera& camera, const std::vector<vimcube::geometry::Line3d>& lines, ftxui::Canvas& canvas, std::vector<ftxui::Color> colors)
    {
        size_t i = 0;
        for (const auto& l : lines)
        {
            auto pt1 = l.start_;
            auto pt2 = l.end_;

            vimcube::geometry::Point2d point1 = camera.projectToCanvas(modelMatrix, pt1, canvas.width(), canvas.height());
            vimcube::geometry::Point2d point2 = camera.projectToCanvas(modelMatrix, pt2, canvas.width(), canvas.height());

            if(!clipCanvas(point1, point2, canvas))
                continue;

            int screenX1 = static_cast<int>(point1.x);
            int screenY1 = static_cast<int>(point1.y);
            int screenX2 = static_cast<int>(point2.x);
            int screenY2 = static_cast<int>(point2.y);

            canvas.DrawPointLine(screenX1, screenY1, screenX2, screenY2, colors.at(i % colors.size()));

            i++;
        }
    }
}
