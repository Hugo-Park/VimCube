#ifndef TOLERANCE_H
#define TOLERANCE_H

namespace vimcube::boolean
{
    static constexpr float EPSILON_WELD_DISTANCE = 0.001f;
    static constexpr float EPSILON_WELD_DISTANCE_SQUARED = EPSILON_WELD_DISTANCE * EPSILON_WELD_DISTANCE;
    static constexpr float EPSILON_PLANE_DISTANCE = 0.0001f;
    static constexpr float EPSILON_DEGEN_TRIANGLE_RATIO = 0.0001f;
}

#endif
