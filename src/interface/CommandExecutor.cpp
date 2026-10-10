#include "CommandExecutor.h"
#include <cstdint>

namespace vimcube::command_executor {
    
    namespace {
        static constexpr float PI = 3.14159265f;

        static constexpr float RADIUS = 10.0f;
        static constexpr uint32_t RINGS = 10;
        static constexpr uint32_t SEGMENTS = 10;

        ExecuteResult check(interface::VimCubeApp& app, const std::vector<vimcube::command::Command>& cmd)
        {
            bool hasSelection = app.getSelectedGeo() != nullptr;
            size_t i = 0;

            for (auto& c : cmd)
            {
                switch (c.op)
                {
                case (command::Op::CreateCube):
                    if (hasSelection)
                    {
                        return ExecuteResult(false, "cannot create geometry while an object is selected", i);
                    }
                    if (c.value <= 0)
                    {
                        return ExecuteResult(false, "size must be positive value", i);
                    }
                    hasSelection = true;
                    break;

                case (command::Op::CreateSphere):
                    if (hasSelection)
                    {
                        return ExecuteResult(false, "cannot create geometry while an object is selected", i);
                    }
                    if (c.value <= 0)
                    {
                        return ExecuteResult(false, "radius must be positive value", i);
                    }
                    hasSelection = true;
                    break;

                case (command::Op::Move):
                    if (!hasSelection)
                    {
                        return ExecuteResult(false, "No selected objects", i);
                    }
                    break;

                case (command::Op::Scale):
                    if (!hasSelection)
                    {
                        return ExecuteResult(false, "No selected objects", i);
                    }
                    if (c.value == 0)
                    {
                        return ExecuteResult(false, "scale factor cannot be zero", i);
                    }
                    break;

                case (command::Op::ScaleUniform):
                    if (!hasSelection)
                    {
                        return ExecuteResult(false, "No selected objects", i);
                    }
                    if (c.value == 0)
                    {
                        return ExecuteResult(false, "scale factor cannot be zero", i);
                    }
                    break;

                case (command::Op::Rotate):
                    if (!hasSelection)
                    {
                        return ExecuteResult(false, "No selected objects", i);
                    }
                    break;

                case (command::Op::Nothing):
                    return ExecuteResult(false, "unsupported command", i);

                case (command::Op::Delete):
                    return ExecuteResult(false, "unsupported command", i);
                }
                i++;
            }
            return ExecuteResult(true);
        }
    }

    ExecuteResult execute(interface::VimCubeApp& app, const std::vector<vimcube::command::Command>& cmd)
    {
        ExecuteResult result = check(app, cmd);
        if(!result.ok)
            return result;

        for (auto& c : cmd)
        {
            switch (c.op)
            {
                case (command::Op::CreateCube):
                {
                    vimcube::geometry::Mesh cube = vimcube::geo_factory::createCube(c.value);
                    app.selectGeo(app.addGeoToScene(cube));
                    break;
                }

                case (command::Op::CreateSphere):
                {
                    vimcube::geometry::Mesh sphere = vimcube::geo_factory::createSphere(c.value, RINGS, SEGMENTS);
                    app.selectGeo(app.addGeoToScene(sphere));
                    break;
                }

                case (command::Op::Move):
                {
                    vimcube::geometry::SceneGeometry* selectedGeo = app.getSelectedGeo();
                    selectedGeo->getTransform().translate(c.axis, c.value);
                    break;
                }

                case (command::Op::Scale):
                {
                    vimcube::geometry::SceneGeometry* selectedGeo = app.getSelectedGeo();
                    selectedGeo->getTransform().scale(c.axis, c.value);
                    break;
                }

                case (command::Op::ScaleUniform):
                {
                    vimcube::geometry::SceneGeometry* selectedGeo = app.getSelectedGeo();
                    selectedGeo->getTransform().scale(vimcube::math::Axis::X, c.value);
                    selectedGeo->getTransform().scale(vimcube::math::Axis::Y, c.value);
                    selectedGeo->getTransform().scale(vimcube::math::Axis::Z, c.value);
                    break;
                }

                case (command::Op::Rotate):
                {
                    vimcube::geometry::SceneGeometry* selectedGeo = app.getSelectedGeo();
                    float radian = c.value * PI / 180.0f;
                    selectedGeo->getTransform().rotate(c.axis, radian);
                    break;
                }

                case (command::Op::Nothing):
                    break;

                case (command::Op::Delete):
                    break;
            }
        }
        return ExecuteResult(true);
    }
}
