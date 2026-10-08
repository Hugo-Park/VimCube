#include "SceneGeometry.h"

namespace vimcube::geometry {

    /*
        Function Name : SceneGeometry
        Parameters : uint32_t id, const vimcube::geometry::Mesh& mesh
        Return Type : -
        Description : Constructor for SceneGeometry class
    */
    SceneGeometry::SceneGeometry(uint32_t id, const vimcube::geometry::Mesh& mesh)
        : id_(id), mesh_(mesh) {}

    /*
        Function Name : SceneGeometry
        Parameters : uint32_t id, const vimcube::geometry::Mesh& mesh, const vimcube::geometry::Transform& transform
        Return Type : -
        Description : Constructor for SceneGeometry class
    */
    SceneGeometry::SceneGeometry(uint32_t id, const vimcube::geometry::Mesh& mesh, const vimcube::geometry::Transform& transform)
        : id_(id), mesh_(mesh), transform_(transform) {}

    /*
        Function Name : getId
        Parameters : -
        Return Type : uint32_t
        Description : Return object id
    */
    uint32_t SceneGeometry::getId() const
    {
        return this->id_;
    }

    /*
        Function Name : getIsSelected
        Parameters : -
        Return Type : bool
        Description : Return isSelected
    */
    bool SceneGeometry::getIsSelected() const
    {
        return this->isSelected_;
    }

    /*
        Function Name : getMesh
        Parameters : -
        Return Type : const vimcube::geometry::Mesh&
        Description : Return mesh
    */
    const vimcube::geometry::Mesh& SceneGeometry::getMesh() const
    {
        return this->mesh_;
    }

    /*
        Function Name : getTransform
        Parameters : -
        Return Type : const vimcube::geometry::Transform&
        Description : Return transform
    */
    const vimcube::geometry::Transform& SceneGeometry::getTransform() const
    {
        return this->transform_;
    }

    /*
        Function Name : getTransform
        Parameters : -
        Return Type : vimcube::geometry::Transform&
        Description : Return transform
    */
    vimcube::geometry::Transform& SceneGeometry::getTransform()
    {
        return this->transform_;
    }

    /*
        Function Name : setIsSelected
        Parameters : bool state
        Return Type : void
        Description : Set isSelected
    */
    void SceneGeometry::setIsSelected(bool state)
    {
        this->isSelected_ = state;
    }
}
