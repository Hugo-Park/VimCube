#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "SceneGeometry.h"

namespace vimcube::geometry
{
    struct Scene
    {
        std::vector<vimcube::geometry::SceneGeometry> sceneGeos;
        uint32_t nextId = 0;
    };
}

#endif
