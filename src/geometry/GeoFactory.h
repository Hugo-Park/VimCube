#ifndef GEO_FACTORY_H
#define GEO_FACTORY_H
#include "GeoTypes.h"
#include "Mesh.h"

/*
    Namespace : geo_factory 
    Description : geo_factory namespace's methods construct Geometry types' data.
*/
namespace vimcube::geo_factory {
    vimcube::geometry::Mesh createCube(float size);
}
#endif