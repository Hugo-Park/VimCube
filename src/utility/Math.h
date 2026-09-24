#ifndef MATH_H
#define MATH_H
#include "../geometry/GeoTypes.h"

namespace vimcube::math {

    struct Matrix3by3
    {
        float entries[3][3];
        Matrix3by3() = default;
    };

    struct Matrix4by4
    {
        float entries[4][4];
        Matrix4by4() = default;
    };

    /* Matrix Methods */

    void initMatrix3by3(Matrix3by3& m);
    void initMatrix4by4(Matrix4by4& m);

    Matrix3by3 constructMatrix3by3(const float(&entries)[3][3]);
    Matrix4by4 constructMatrix4by4(const float(&entries)[4][4]);

    float getDet3by3(const Matrix3by3& m);
    float getDet4by4(const Matrix4by4& m);

    Matrix3by3 getProduct3by3(const Matrix3by3& m1, const Matrix3by3& m2);
    Matrix4by4 getProduct4by4(const Matrix4by4& m1, const Matrix4by4& m2);

    /* Vector Methods */

    vimcube::geometry::Point4d castToPoint4d(const vimcube::geometry::Point3d& p);
    vimcube::geometry::Vector4d castToVector4d(const vimcube::geometry::Vector3d& v);

    vimcube::geometry::Point4d getProduct4by4andPoint4d(const Matrix4by4& m, const vimcube::geometry::Point4d p);
    vimcube::geometry::Vector4d getProduct4by4andVector4d(const Matrix4by4& m, const vimcube::geometry::Vector4d v);

    vimcube::geometry::Vector3d subtractVector3d(const vimcube::geometry::Vector3d& v1, const vimcube::geometry::Vector3d& v2);
    vimcube::geometry::Vector4d subtractVector4d(const vimcube::geometry::Vector4d& v1, const vimcube::geometry::Vector4d& v2);

    float dotProductVector3d(const vimcube::geometry::Vector3d& v1, const vimcube::geometry::Vector3d& v2);
    float dotProductVector4d(const vimcube::geometry::Vector4d& v1, const vimcube::geometry::Vector4d& v2);

    vimcube::geometry::Vector3d crossProductVector3d(const vimcube::geometry::Vector3d& v1, const vimcube::geometry::Vector3d& v2);

    vimcube::geometry::Vector3d normalizeVector3d(const vimcube::geometry::Vector3d& v);
    vimcube::geometry::Vector4d normalizeVector4d(const vimcube::geometry::Vector4d& v);

    /* Build Matrix for MVP Methods*/

    Matrix4by4 buildOrthogonalMatrix(float zoom, float aspect, float nearClip, float farClip);
    Matrix4by4 buildPerspectiveMatrix(float fieldOfView, float aspect, float nearClip, float farClip);
    Matrix4by4 buildViewMatrix(vimcube::geometry::Vector3d& eye, vimcube::geometry::Vector3d& target, vimcube::geometry::Vector3d& up);
}
#endif
