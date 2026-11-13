#ifndef BOOLEAN_H
#define BOOLEAN_H
#include <string>
#include "../geometry/Mesh.h"

namespace vimcube::boolean
{
    struct WaterTightResult
    {
        bool isClosed = false;
        uint32_t openEdgeNum = 0;
        uint32_t nonManifoldEdgeNum = 0;
        std::string errorMessage;
    };

    WaterTightResult checkIsMeshClosed(const vimcube::geometry::Mesh& mesh);
}

#endif
