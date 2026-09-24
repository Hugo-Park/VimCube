#include "Math.h"

namespace vimcube::math {
    
    /*
        Function Name : initMatrix3by3
        Parameters : Matrix3by3& m
        Return Type : void
        Description : Initializes Matrix3by3
    */
    void initMatrix3by3(Matrix3by3& m)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                m.entries[i][j] = 0;
    }

    /*
        Function Name : initMatrix4by4
        Parameters : Matri4by4& m
        Return Type : void
        Description : Initializes Matrix4by4
    */
    void initMatrix4by4(Matrix4by4& m)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                m.entries[i][j] = 0;
    }

    /*
        Function Name : constructMatrix3by3
        Parameters : const float(&entries)[3][3]
        Return Type : Matrix3by3
        Description : Constructs Matrix3by3
    */
    Matrix3by3 constructMatrix3by3(const float(&entries)[3][3])
    {
        Matrix3by3 returnMatrix;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                returnMatrix.entries[i][j] = entries[i][j];
            }
        }
        return returnMatrix;
    }

    /*
        Function Name : constructMatrix4by4
        Parameters : const float(&entries)[4][4]
        Return Type : Matrix4by4
        Description : Constructs Matrix4by4
    */
    Matrix4by4 constructMatrix4by4(const float(&entries)[4][4])
    {
        Matrix4by4 returnMatrix;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                returnMatrix.entries[i][j] = entries[i][j];
            }
        }
        return returnMatrix;
    }

    /*
        Function Name : getDet3by3
        Parameters : const Matrix3by3& m
        Return Type : float
        Description : Get determinant of Matrix3by3
    */
    float getDet3by3(const Matrix3by3& m)
    {
        float cofactor1 = m.entries[0][0] * (m.entries[1][1] * m.entries[2][2] - m.entries[1][2] * m.entries[2][1]);
        float cofactor2 = m.entries[0][1] * (m.entries[1][0] * m.entries[2][2] - m.entries[1][2] * m.entries[2][0]);
        float cofactor3 = m.entries[0][2] * (m.entries[1][0] * m.entries[2][1] - m.entries[1][1] * m.entries[2][0]);
        return cofactor1 - cofactor2 + cofactor3;
    }

    /*
        Function Name : getDet4by4
        Parameters : const Matrix4by4& m
        Return Type : float
        Description : Get determinant of Matrix4by4
    */
    float getDet4by4(const Matrix4by4& m)
    {
        float entries1[3][3] = { {m.entries[1][1], m.entries[1][2], m.entries[1][3]}, {m.entries[2][1], m.entries[2][2], m.entries[2][3]}, {m.entries[3][1], m.entries[3][2], m.entries[3][3]} };
        float cofactor1 = m.entries[0][0] * getDet3by3(constructMatrix3by3(entries1));

        float entries2[3][3] = { {m.entries[1][0], m.entries[1][2], m.entries[1][3]}, {m.entries[2][0], m.entries[2][2], m.entries[2][3]}, {m.entries[3][0], m.entries[3][2], m.entries[3][3]} };
        float cofactor2 = m.entries[0][1] * getDet3by3(constructMatrix3by3(entries2));

        float entries3[3][3] = { {m.entries[1][0], m.entries[1][1], m.entries[1][3]}, {m.entries[2][0], m.entries[2][1], m.entries[2][3]}, {m.entries[3][0], m.entries[3][1], m.entries[3][3]} };
        float cofactor3 = m.entries[0][2] * getDet3by3(constructMatrix3by3(entries3));

        float entries4[3][3] = { {m.entries[1][0], m.entries[1][1], m.entries[1][2]}, {m.entries[2][0], m.entries[2][1], m.entries[2][2]}, {m.entries[3][0], m.entries[3][1], m.entries[3][2]} };
        float cofactor4 = m.entries[0][3] * getDet3by3(constructMatrix3by3(entries4));

        return cofactor1 - cofactor2 + cofactor3 - cofactor4;
    }

    /*
        Function Name : getProduct3by3
        Parameters : const Matrix3by3& m1, const Matrix3by3& m2
        Return Type : Matrix3by3
        Description : Get product of two Matrix3by3
    */
    Matrix3by3 getProduct3by3(const Matrix3by3& m1, const Matrix3by3& m2)
    {
        Matrix3by3 returnMatrix;
        initMatrix3by3(returnMatrix);

        for (int i = 0; i < 3; i++)
            for (int k = 0; k < 3; k++)
                for (int j = 0; j < 3; j++)
                    returnMatrix.entries[i][j] += m1.entries[i][k] * m2.entries[k][j];

        return returnMatrix;
    }

    /*
        Function Name : getProduct4by4
        Parameters : const Matrix4by4& m1, const Matrix4by4& m2
        Return Type : Matrix4by4
        Description : Get product of two Matrix4by4
    */
    Matrix4by4 getProduct4by4(const Matrix4by4& m1, const Matrix4by4& m2)
    {
        Matrix4by4 returnMatrix;
        initMatrix4by4(returnMatrix);

        for (int i = 0; i < 4; i++)
            for (int k = 0; k < 4; k++)
                for (int j = 0; j < 4; j++)
                    returnMatrix.entries[i][j] += m1.entries[i][k] * m2.entries[k][j];

        return returnMatrix;
    }

    /*
        Function Name : castToPoint4d
        Parameters : const vimcube::geometry::Point4d& p
        Return Type : vimcube::geometry::Point4d
        Description : Cast to Point4d
    */
    vimcube::geometry::Point4d castToPoint4d(const vimcube::geometry::Point3d& p)
    {
        vimcube::geometry::Point4d returnPoint;

        returnPoint.x = p.x;
        returnPoint.y = p.y;
        returnPoint.z = p.z;

        return returnPoint;
    }

    /*
        Function Name : castToVector4d
        Parameters : const vimcube::geometry::Vector3d& v
        Return Type : vimcube::geometry::Vector4d
        Description : Cast to Vector4d
    */
    vimcube::geometry::Vector4d castToVector4d(const vimcube::geometry::Vector3d& v)
    {
        vimcube::geometry::Vector4d returnVector;

        returnVector.x = v.x;
        returnVector.y = v.y;
        returnVector.z = v.z;

        return returnVector;
    }

    /*
        Function Name : getProduct4by4andPoint4d
        Parameters : const Matrix4by4& m, const vimcube::geometry::Point4d p
        Return Type : vimcube::geometry::Point4d
        Description : Get product of Matrix4by4 and Point4d
    */
    vimcube::geometry::Point4d getProduct4by4andPoint4d(const Matrix4by4& m, const vimcube::geometry::Point4d p)
    {
        vimcube::geometry::Point4d returnPoint;

        returnPoint.x = m.entries[0][0] * p.x + m.entries[0][1] * p.y + m.entries[0][2] * p.z + m.entries[0][3] * p.w;
        returnPoint.y = m.entries[1][0] * p.x + m.entries[1][1] * p.y + m.entries[1][2] * p.z + m.entries[1][3] * p.w;
        returnPoint.z = m.entries[2][0] * p.x + m.entries[2][1] * p.y + m.entries[2][2] * p.z + m.entries[2][3] * p.w;
        returnPoint.w = m.entries[3][0] * p.x + m.entries[3][1] * p.y + m.entries[3][2] * p.z + m.entries[3][3] * p.w;

        return returnPoint;
    }

    /*
        Function Name : getProduct4by4andVector4d
        Parameters : const Matrix4by4& m, const vimcube::geometry::Vector4d v
        Return Type : vimcube::geometry::Vector4d
        Description : Get product of Matrix4by4 and Vector4d
    */
    vimcube::geometry::Vector4d getProduct4by4andVector4d(const Matrix4by4& m, const vimcube::geometry::Vector4d v)
    {
        vimcube::geometry::Vector4d returnVector;

        returnVector.x = m.entries[0][0] * v.x + m.entries[0][1] * v.y + m.entries[0][2] * v.z + m.entries[0][3] * v.w;
        returnVector.y = m.entries[1][0] * v.x + m.entries[1][1] * v.y + m.entries[1][2] * v.z + m.entries[1][3] * v.w;
        returnVector.z = m.entries[2][0] * v.x + m.entries[2][1] * v.y + m.entries[2][2] * v.z + m.entries[2][3] * v.w;
        returnVector.w = m.entries[3][0] * v.x + m.entries[3][1] * v.y + m.entries[3][2] * v.z + m.entries[3][3] * v.w;

        return returnVector;
    }


    /*
        Function Name : subtractVector3d
        Parameters : const vimcube::geometry::Vector3d& v1, const vimcube::geometry::Vector3d& v2
        Return Type : vimcube::geometry::Vector3d
        Description : Subtract Vector3d to other Vector3d
    */
    vimcube::geometry::Vector3d subtractVector3d(const vimcube::geometry::Vector3d& v1, const vimcube::geometry::Vector3d& v2)
    {
        vimcube::geometry::Vector3d returnVector;

        returnVector.x = v1.x - v2.x;
        returnVector.y = v1.y - v2.y;
        returnVector.z = v1.z - v2.z;

        return returnVector;
    }

    /*
        Function Name : subtractVector4d
        Parameters : const vimcube::geometry::Vector4d& v1, const vimcube::geometry::Vector4d& v2
        Return Type : vimcube::geometry::Vector4d
        Description : Subtract Vector4d from other Vector4d
    */
    vimcube::geometry::Vector4d subtractVector4d(const vimcube::geometry::Vector4d& v1, const vimcube::geometry::Vector4d& v2)
    {
        vimcube::geometry::Vector4d returnVector;

        returnVector.x = v1.x - v2.x;
        returnVector.y = v1.y - v2.y;
        returnVector.z = v1.z - v2.z;
        returnVector.w = v1.w - v2.w;

        return returnVector;
    }

    /*
        Function Name : dotProductVector3d
        Parameters : const vimcube::geometry::Vector3d& v1, const vimcube::geometry::Vector3d& v2
        Return Type : vimcube::geometry::Vector3d
        Description : Get dot product of Vector3d
    */
    float dotProductVector3d(const vimcube::geometry::Vector3d& v1, const vimcube::geometry::Vector3d& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    /*
        Function Name : dotProductVector4d
        Parameters : const vimcube::geometry::Vector4d& v1, const vimcube::geometry::Vector4d& v2
        Return Type : vimcube::geometry::Vector4d
        Description : Get dot product of Vector4d
    */
    float dotProductVector4d(const vimcube::geometry::Vector4d& v1, const vimcube::geometry::Vector4d& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    /*
        Function Name : crossProductVector3d
        Parameters : const vimcube::geometry::Vector3d& v1, const vimcube::geometry::Vector3d& v2
        Return Type : vimcube::geometry::Vector3d
        Description : Get cross product of Vector3d
    */
    vimcube::geometry::Vector3d crossProductVector3d(const vimcube::geometry::Vector3d& v1, const vimcube::geometry::Vector3d& v2)
    {
        vimcube::geometry::Vector3d returnVector;

        returnVector.x = v1.y * v2.z - v1.z * v2.y;
        returnVector.y = v1.z * v2.x - v1.x * v2.z;
        returnVector.z = v1.x * v2.y - v1.y * v2.x;

        return returnVector;
    }

    /*
        Function Name : normalizeVector3d
        Parameters : vimcube::geometry::Vector3d& v
        Return Type : vimcube::geometry::Vector3d
        Description : Normalize Vector3d
    */
    vimcube::geometry::Vector3d normalizeVector3d(const vimcube::geometry::Vector3d& v)
    {
        vimcube::geometry::Vector3d returnVector;

        returnVector.x = float(v.x / v.getSize());
        returnVector.y = float(v.y / v.getSize());
        returnVector.z = float(v.z / v.getSize());

        return returnVector;
    }

    /*
        Function Name : normalizeVector4d
        Parameters : vimcube::geometry::Vector4d& v
        Return Type : vimcube::geometry::Vector4d
        Description : Normalize Vector4d
    */
    vimcube::geometry::Vector4d normalizeVector4d(const vimcube::geometry::Vector4d& v)
    {
        vimcube::geometry::Vector4d returnVector;

        returnVector.x = float(v.x / v.getSize());
        returnVector.y = float(v.y / v.getSize());
        returnVector.z = float(v.z / v.getSize());
        returnVector.w = float(v.w / v.getSize());

        return returnVector;
    }

    /*
        Function Name : buildOrthogonalMatrix
        Parameters : float zoom, float aspect, float nearClip, float farClip
        Return Type : Matrix4by4
        Description : Build orthogonal matrix for MVP methods
    */
    Matrix4by4 buildOrthogonalMatrix(float zoom, float aspect, float nearClip, float farClip)
    {
        Matrix4by4 orthoMatrix;
        initMatrix4by4(orthoMatrix);

        orthoMatrix.entries[0][0] = float(zoom / aspect);
        orthoMatrix.entries[1][1] = float(zoom);
        orthoMatrix.entries[2][2] = float(2 / (farClip - nearClip));
        orthoMatrix.entries[2][3] = float(-1 - 2 * (nearClip / (farClip - nearClip)));
        orthoMatrix.entries[3][3] = 1.0f;

        return orthoMatrix;
    }

    /*
        Function Name : buildPerspectiveMatrix
        Parameters : float fieldOfView, float aspect, float nearClip, float farClip
        Return Type : Matrix4by4
        Description : Build perspective matrix for MVP methods
    */
    Matrix4by4 buildPerspectiveMatrix(float fieldOfView, float aspect, float nearClip, float farClip)
    {
        Matrix4by4 perspectMatrix;
        initMatrix4by4(perspectMatrix);
        float f = float(1.0f / tan(fieldOfView / 2));
        
        perspectMatrix.entries[0][0] = f / aspect;
        perspectMatrix.entries[1][1] = f;
        perspectMatrix.entries[2][2] = (farClip + nearClip) / (nearClip - farClip);
        perspectMatrix.entries[2][3] = (2 * farClip * nearClip) / (nearClip - farClip);
        perspectMatrix.entries[3][2] = -1.0f;

        return perspectMatrix;
    }
    
    /*
        Function Name : buildViewMatrix
        Parameters : vimcube::geometry::Vector3d eye, vimcube::geometry::Vector3d target, vimcube::geometry::Vector3d up
        Return Type : Matrix4by4
        Description : Build view matrix for MVP methods
    */
    Matrix4by4 buildViewMatrix(vimcube::geometry::Vector3d& eye, vimcube::geometry::Vector3d& target, vimcube::geometry::Vector3d& up)
    {
        Matrix4by4 viewMatrix;
        initMatrix4by4(viewMatrix);

        vimcube::geometry::Vector3d forward = normalizeVector3d(subtractVector3d(target, eye));
        vimcube::geometry::Vector3d right = normalizeVector3d(crossProductVector3d(forward, up));
        vimcube::geometry::Vector3d trueUp = crossProductVector3d(right, forward);

        viewMatrix.entries[0][0] = right.x;
        viewMatrix.entries[0][1] = right.y;
        viewMatrix.entries[0][2] = right.z;
        viewMatrix.entries[0][3] = -1.0f * dotProductVector3d(eye, right);
        viewMatrix.entries[1][0] = trueUp.x;
        viewMatrix.entries[1][1] = trueUp.y;
        viewMatrix.entries[1][2] = trueUp.z;
        viewMatrix.entries[1][3] = -1.0f * dotProductVector3d(eye, trueUp);
        viewMatrix.entries[2][0] = forward.x;
        viewMatrix.entries[2][1] = forward.y;
        viewMatrix.entries[2][2] = forward.z;
        viewMatrix.entries[2][3] = -1.0f * dotProductVector3d(eye, forward);
        viewMatrix.entries[3][3] = 1.0f;

        return viewMatrix;
    }
}
