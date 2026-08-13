#include "GeoDraw.h"

namespace vimcube::geo_draw {
    /*
        Function Name : draw 
        Parameters : vimcube::geometry::Mesh& mesh, ftxui::Canvas& canvas
        Return Type : void
        Description : Draw a Mesh object
    */
    void draw(const vimcube::geometry::Mesh& mesh, ftxui::Canvas& canvas)
    {
        // Set canvas center point
        float centerX = canvas.width() / 2.0f;
        float centerY = canvas.height()/ 2.0f;

        for (const auto& edge : mesh.getEdges())
        {
            // Get real coordinates of vertices
            auto pt1 = mesh.getVertices()[edge.v0].position;
            auto pt2 = mesh.getVertices()[edge.v1].position;

            /*
            int screenX1 = static_cast<int>(pt1.x + centerX);
            int screenY1 = static_cast<int>(pt1.y + centerY);
            int screenX2 = static_cast<int>(pt2.x + centerX);
            int screenY2 = static_cast<int>(pt2.y + centerY);
            */
            
            // Convert to 2D points --> MVP method later
            float scale = 1.0f;
            int screenX1 = static_cast<int>((pt1.x + pt1.z * 0.5f) * scale + centerX);
            int screenY1 = static_cast<int>((pt1.y + pt1.z * 0.5f) * scale + centerY);
            int screenX2 = static_cast<int>((pt2.x + pt2.z * 0.5f) * scale + centerX);
            int screenY2 = static_cast<int>((pt2.y + pt2.z * 0.5f) * scale + centerY);
            
            canvas.DrawPointLine(screenX1, screenY1, screenX2, screenY2, ftxui::Color::White);
        }

    }
}