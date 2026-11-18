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

    struct RemoveDegenerateResult
    {
        vimcube::geometry::Mesh mesh;
        uint32_t removedFacesNum = 0;
    };

    struct CleanUpResult
    {
        vimcube::geometry::Mesh mesh;
        uint32_t mergedVerticesNum = 0;
        uint32_t removedFacesNum = 0;
    };

    WeldResult weldVertices(const vimcube::geometry::Mesh& m);
    RemoveDegenerateResult removeDegenerateTriangles(const vimcube::geometry::Mesh& m);

    CleanUpResult cleanUpMesh(const vimcube::geometry::Mesh& m);
}
#endif
