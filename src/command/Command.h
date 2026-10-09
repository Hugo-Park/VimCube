#ifndef COMMAND_H
#define COMMAND_H

#include "../utility/Math.h"

namespace vimcube::command {

    enum class Op { Nothing, CreateCube, CreateSphere, Move, Scale, ScaleUniform, Rotate, Delete };

    struct Command
    {
        Op op = Op::Nothing;
        vimcube::math::Axis axis = vimcube::math::Axis::X;
        float value = 0.0f;
    };
}
#endif
