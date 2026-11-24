#ifndef TOLERANCE_H
#define TOLERANCE_H

namespace vimcube::boolean
{
    static constexpr float EPSILON_WELD_DISTANCE = 1e-3f;
    static constexpr float EPSILON_WELD_DISTANCE_SQUARED = EPSILON_WELD_DISTANCE * EPSILON_WELD_DISTANCE;
    static constexpr float EPSILON_PLANE_DISTANCE = 1e-4f;
    static constexpr float EPSILON_DEGEN_TRIANGLE_RATIO = 1e-4f;
    static constexpr float EPSILON_AABB_MARGIN = 1e-4f;
}

#endif
