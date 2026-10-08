#ifndef SCENE_GEOMETRY_H
#define SCENE_GEOMETRY_H

#include <cstdint>
#include "Mesh.h"
#include "Transform.h"

namespace vimcube::geometry {

    class SceneGeometry
    {
    private:
        uint32_t id_;
        bool isSelected_ = false;
        vimcube::geometry::Mesh mesh_;
        vimcube::geometry::Transform transform_;

    public:
        SceneGeometry(uint32_t id, const vimcube::geometry::Mesh& mesh);
        SceneGeometry(uint32_t id, const vimcube::geometry::Mesh& mesh, const vimcube::geometry::Transform& transform);

        uint32_t getId() const;
        bool getIsSelected() const;
        const vimcube::geometry::Mesh& getMesh() const;
        const vimcube::geometry::Transform& getTransform() const;

        vimcube::geometry::Transform& getTransform();

        void setIsSelected(bool state);
    };
}

#endif
