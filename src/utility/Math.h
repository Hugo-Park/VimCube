#ifndef MATH_H
#define MATH_H

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

    void initMatrix3by3(Matrix3by3& m);
    void initMatrix4by4(Matrix4by4& m);
    Matrix3by3 constructMatrix3By3(const float(&entries)[3][3]);
    Matrix4by4 constructMatrix4by4(const float(&entries)[4][4]);

    float getDet3by3(const Matrix3by3& m);
    float getDet4by4(const Matrix4by4& m);

    Matrix3by3 getProduct3by3(const Matrix3by3& m1, const Matrix3by3& m2);
    Matrix4by4 getProduct4by4(const Matrix4by4& m1, const Matrix4by4& m2);
}
#endif
