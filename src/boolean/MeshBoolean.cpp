#include "MeshBoolean.h"
#include "../geometry/GeoTypes.h"
#include "Boolean.h"
#include "GeometryUtils.h"
#include <cstdint>
#include <vector>
#include <cmath>

namespace vimcube::boolean
{
    namespace
    {
        /*
            Function Name : computeTriangleCentroid
            Parameters : const vimcube::geometry::Triangle& t
            Return Type : vimcube::geometry::Point3d
            Description : Compute center of gravity of triangle
        */
        vimcube::geometry::Point3d computeTriangleCentroid(const vimcube::geometry::Triangle& t)
        {
            return vimcube::geometry::Point3d((t.p0.x + t.p1.x + t.p2.x) / 3.0f, (t.p0.y + t.p1.y + t.p2.y) / 3.0f, (t.p0.z + t.p1.z + t.p2.z) / 3.0f);
        }

        /*
            Function Name : isSurfaceIntersecting
            Parameters : const vimcube::geometry::Mesh& a, const vimcube::geometry::Mesh& b
            Return Type : bool
            Description : Check if surfaces intersect
        */
        bool isSurfaceIntersecting(const vimcube::geometry::Mesh& a, const vimcube::geometry::Mesh& b)
        {
            for (const auto& fA : a.getFaces())
            {
                for (const auto& fB : b.getFaces())
                {
                    TriangleIntersection result = classifyTriangleIntersection(getTriangleFromFace(a, fA), getTriangleFromFace(b, fB));
                    if (result.relation == TriangleRelation::COPLANAR) return true;
                    if (result.relation == TriangleRelation::SEGMENT) return true;
                }
            }
            return false;
        }

        /*
            Function Name : classifyFacesAgainstMesh
            Parameters : const vimcube::geometry::Mesh& target, const vimcube::geometry::Mesh& reference
            Return Type : std::vector<PointRelation>
            Description : Classify faces of target mesh against reference mesh
        */
        std::vector<PointRelation> classifyFacesAgainstMesh(const vimcube::geometry::Mesh& target, const vimcube::geometry::Mesh& reference)
        {
            std::vector<PointRelation> result;

            for (const auto& f : target.getFaces())
            {
                PointRelation r = classifyPointRelation(computeTriangleCentroid(getTriangleFromFace(target, f)), reference);
                result.push_back(r);
            }
            return result;
        }

        /*
            Function Name : assembleBooleanMesh
            Parameters : const vimcube::geometry::Mesh& a, const vimcube::geometry::Mesh& b, const std::vector<PointRelation>& relationsA, const std::vector<PointRelation>& relationsB, BooleanOp op
            Return Type : vimcube::geometry::Mesh
            Description : Assemble mesh as boolean result
        */
        vimcube::geometry::Mesh assembleBooleanMesh(const vimcube::geometry::Mesh& a, const vimcube::geometry::Mesh& b, const std::vector<PointRelation>& relationsA, const std::vector<PointRelation>& relationsB, BooleanOp op)
        {
            std::vector<vimcube::geometry::Vertex> newVertices;
            std::vector<vimcube::geometry::Vertex> vA = a.getVertices();
            std::vector<vimcube::geometry::Vertex> vB = b.getVertices();

            // push back to new vertices list
            uint32_t offset = 0;
            for (size_t i = 0; i < vA.size(); i++)
            {
                newVertices.push_back(vA.at(i));
                offset++;
            }
            for (size_t i = 0; i < vB.size(); i++)
            {
                newVertices.push_back(vB.at(i));
            }

            std::vector<vimcube::geometry::Face> newFaces;
            size_t i = 0;

            // classify which faces should remain by boolean operations
            for (const auto& fA : a.getFaces())
            {
                if (op == BooleanOp::UNION)
                {
                    if (relationsA.at(i) == PointRelation::OUTSIDE)
                    {
                        newFaces.push_back(fA);
                    }
                }
                else if (op == BooleanOp::INTERSECTION)
                {
                    if (relationsA.at(i) == PointRelation::INSIDE)
                    {
                        newFaces.push_back(fA);
                    }
                }
                else
                {
                    if (relationsA.at(i) == PointRelation::OUTSIDE)
                    {
                        newFaces.push_back(fA);
                    }
                }
                i++;
            }

            i = 0;
            for (const auto& fB : b.getFaces())
            {
                if (op == BooleanOp::UNION)
                {
                    if (relationsB.at(i) == PointRelation::OUTSIDE)
                    {
                        newFaces.push_back({fB.v0 + offset, fB.v1 + offset, fB.v2 + offset});
                    }
                }
                else if (op == BooleanOp::INTERSECTION)
                {
                    if (relationsB.at(i) == PointRelation::INSIDE)
                    {
                        newFaces.push_back({fB.v0 + offset, fB.v1 + offset, fB.v2 + offset});
                    }
                }
                else
                {
                    if (relationsB.at(i) == PointRelation::INSIDE)
                    {
                        newFaces.push_back({fB.v0 + offset, fB.v2 + offset, fB.v1 + offset});
                    }
                }
                i++;
            }

            // assemble mesh
            vimcube::geometry::Mesh newMesh(newFaces, newVertices, std::vector<vimcube::geometry::Edge>());
            newMesh.setEdgesByFaces();
            return newMesh;
        }

        std::string validateMesh(const vimcube::geometry::Mesh& m, const std::string& label)
        {
            if (m.getFaces().empty())
            {
                return label + ": " + "Empty mesh faces";
            }

            WaterTightResult w = checkIsMeshClosed(m);
            if (!w.isClosed)
            {
                return label + ": " + w.errorMessage;
            }

            WindingResult wi = checkIsWindingConsistent(m);
            if (!wi.isConsistent)
            {
                return label + ": " + wi.errorMessage;
            }

            float signedVol = computeSignedVolume(m);
            if (std::isnan(signedVol))
            {
                return label + ": " + "Mesh has invalid volume";
            }
            if (signedVol <= 0)
            {
                return label + ": " + "Mesh faces point inward(negative volume)";
            }

            return "";
        }
    }

    /*
        Function Name : computeMeshBoolean
        Parameters : const vimcube::geometry::Mesh& a, const vimcube::geometry::Mesh& b, BooleanOp op
        Return Type : BooleanResult
        Description : Execute mesh boolean operation(Parameters should be world meshes)
    */
    BooleanResult computeMeshBoolean(const vimcube::geometry::Mesh& a, const vimcube::geometry::Mesh& b, BooleanOp op)
    {
        BooleanResult result;

        // validate mesh
        std::string errA = validateMesh(a, "a");
        if (!errA.empty())
        {
            result.errorMessage = errA;
            return result;
        }
        std::string errB = validateMesh(b, "b");
        if (!errB.empty())
        {
            result.errorMessage = errB;
            return result;
        }

        if (isSurfaceIntersecting(a, b))
        {
            result.errorMessage = "not support intersecting meshes yet";    // update later
            return result;
        }

        std::vector<PointRelation> relationsA = classifyFacesAgainstMesh(a, b);
        std::vector<PointRelation> relationsB = classifyFacesAgainstMesh(b, a);
        // filter UNCERTAIN cases
        for (const auto& rA : relationsA)
        {
            if (rA == PointRelation::UNCERTAIN)
            {
                result.errorMessage = "Cannot execute boolean operation";
                return result;
            }
        }
        for (const auto& rB : relationsB)
        {
            if (rB == PointRelation::UNCERTAIN)
            {
                result.errorMessage = "Cannot execute boolean operation";
                return result;
            }
        }

        result.mesh = assembleBooleanMesh(a, b, relationsA, relationsB, op);
        return result;
    }
}
