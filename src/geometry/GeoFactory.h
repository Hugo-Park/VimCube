#ifndef GEO_FACTORY_H
#define GEO_FACTORY_H
#include <cstdint>
#include <vector>
#include "GeoTypes.h"
#include "Mesh.h"
#include "../utility/Math.h"

/*
    Namespace : geo_factory 
    Description : geo_factory namespace's methods construct Geometry types' data.
*/
namespace vimcube::geo_factory {
    static constexpr float PI = 3.141592;
    std::vector<vimcube::geometry::Line3d> createAxisIndicator(float length);
    std::vector<vimcube::geometry::Line3d> createGridIndicator(int size);
    vimcube::geometry::Mesh createCube(float size);
    vimcube::geometry::Mesh createSphere(float radius, uint32_t rings, uint32_t segments);
}
#endif
