#ifndef GEO_DRAW_H
#define GEO_DRAW_H
#include <ftxui/dom/canvas.hpp>
#include <ftxui/screen/color.hpp>
#include <vector>
#include "../geometry/GeoTypes.h"
#include "../geometry/Mesh.h"
#include "../render/Camera.h"

/*
    Namespace : geo_draw
    Description : geo_draw nammespace's methods only draw Geometry types(Point, Line, Mesh...).
*/
namespace vimcube::geo_draw {
    void draw(const vimcube::math::Matrix4by4& modelMatrix, vimcube::camera::Camera& camera, const vimcube::geometry::Mesh& mesh, ftxui::Canvas& canvas, ftxui::Color color);
    void draw(const vimcube::math::Matrix4by4& modelMatrix, vimcube::camera::Camera& camera, const vimcube::geometry::Line3d& line, ftxui::Canvas& canvas, ftxui::Color color);
    void draw(const vimcube::math::Matrix4by4& modelMatrix, vimcube::camera::Camera& camera, const std::vector<vimcube::geometry::Line3d>& lines, ftxui::Canvas& canvas, ftxui::Color color);
    void draw(const vimcube::math::Matrix4by4& modelMatrix, vimcube::camera::Camera& camera, const std::vector<vimcube::geometry::Line3d>& lines, ftxui::Canvas& canvas, std::vector<ftxui::Color> colors);
}
#endif
