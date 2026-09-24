#include "GeoDraw.h"
#include "Camera.h"

namespace vimcube::geo_draw {
    /*
        Function Name : draw 
        Parameters : vimcube::geometry::Mesh& mesh, ftxui::Canvas& canvas
        Return Type : void
        Description : Draw a Mesh object
    */
    void draw(vimcube::camera::Camera& camera, const vimcube::geometry::Mesh& mesh, ftxui::Canvas& canvas)
    {

        for (const auto& edge : mesh.getEdges())
        {
            // Get real coordinates of vertices
            auto pt1 = mesh.getVertices()[edge.v0].position;
            auto pt2 = mesh.getVertices()[edge.v1].position;

            vimcube::geometry::Point2d point1 = camera.projectToCanvas(pt1, canvas.width(), canvas.height());
            vimcube::geometry::Point2d point2 = camera.projectToCanvas(pt2, canvas.width(), canvas.height());

            int screenX1 = static_cast<int>(point1.x);
            int screenY1 = static_cast<int>(point1.y);
            int screenX2 = static_cast<int>(point2.x);
            int screenY2 = static_cast<int>(point2.y);
            canvas.DrawPointLine(screenX1, screenY1, screenX2, screenY2, ftxui::Color::White);
        }

    }
}
