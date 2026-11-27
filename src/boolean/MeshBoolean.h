#ifndef MESH_BOOLEAN_H
#define MESH_BOOLEAN_H
#include "../geometry/Mesh.h"
#include <string>

namespace vimcube::boolean
{
    enum class BooleanOp { UNION, INTERSECTION, DIFFERENCE };

    struct BooleanResult
    {
        vimcube::geometry::Mesh mesh;
        std::string errorMessage;
    };

    BooleanResult computeMeshBoolean(const vimcube::geometry::Mesh& a, const vimcube::geometry::Mesh& b, BooleanOp op);
}

#endif
