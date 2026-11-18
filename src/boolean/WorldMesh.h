#ifndef WORLD_MESH_H
#define WORLD_MESH_H

#include <string>
#include "../geometry/Mesh.h"
#include "../geometry/Transform.h"

namespace vimcube::boolean
{
    struct WorldMeshResult
    {
        vimcube::geometry::Mesh mesh;
        std::string errorMessage;
    };

    WorldMeshResult convertToWorldMesh(const vimcube::geometry::Mesh& m, const vimcube::geometry::Transform& t);
}

#endif
