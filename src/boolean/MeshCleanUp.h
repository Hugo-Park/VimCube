#ifndef MESH_CLEAN_UP_H
#define MESH_CLEAN_UP_H
#include <cstdint>
#include "../geometry/Mesh.h"

namespace vimcube::boolean
{
    static constexpr float EPSILON = 0.001f;
    static constexpr float EPSILON_SQUARE = EPSILON * EPSILON;

    struct WeldResult
    {
        vimcube::geometry::Mesh mesh;
        uint32_t mergedVerticesNum = 0;
    };

    WeldResult weldVertices(const vimcube::geometry::Mesh& m);
}
#endif
