#ifndef GEO_DRAW_H
#define GEO_DRAW_H
#include <ftxui/dom/canvas.hpp>
#include "../geometry/GeoTypes.h"
#include "../geometry/Mesh.h"

/*
    Namespace : geo_draw
    Description : geo_draw nammespace's methods only draw Geometry types(Point, Line, Mesh...).
*/
namespace vimcube::geo_draw {
    void draw(vimcube::geometry::Mesh& mesh, ftxui::Canvas& canvas);
}
#endif