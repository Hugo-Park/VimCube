#include "Math.h"

namespace vimcube::math {
    
    void initMatrix3by3(Matrix3by3& m)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                m.entries[i][j] = 0;
    }

    void initMatrix4by4(Matrix4by4& m)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                m.entries[i][j] = 0;
    }

    Matrix3by3 constructMatrix3By3(const float(&entries)[3][3])
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

    float getDet3by3(const Matrix3by3& m)
    {
        float cofactor1 = m.entries[0][0] * (m.entries[1][1] * m.entries[2][2] - m.entries[1][2] * m.entries[2][1]);
        float cofactor2 = m.entries[0][1] * (m.entries[1][0] * m.entries[2][2] - m.entries[1][2] * m.entries[2][0]);
        float cofactor3 = m.entries[0][2] * (m.entries[1][0] * m.entries[2][1] - m.entries[1][1] * m.entries[2][0]);
        return cofactor1 - cofactor2 + cofactor3;
    }

    float getDet4by4(const Matrix4by4& m)
    {
        float entries1[3][3] = { {m.entries[1][1], m.entries[1][2], m.entries[1][3]}, {m.entries[2][1], m.entries[2][2], m.entries[2][3]}, {m.entries[3][1], m.entries[3][2], m.entries[3][3]} };
        float cofactor1 = m.entries[0][0] * getDet3by3(constructMatrix3By3(entries1));

        float entries2[3][3] = { {m.entries[1][0], m.entries[1][2], m.entries[1][3]}, {m.entries[2][0], m.entries[2][2], m.entries[2][3]}, {m.entries[3][0], m.entries[3][2], m.entries[3][3]} };
        float cofactor2 = m.entries[0][1] * getDet3by3(constructMatrix3By3(entries2));

        float entries3[3][3] = { {m.entries[1][0], m.entries[1][1], m.entries[1][3]}, {m.entries[2][0], m.entries[2][1], m.entries[2][3]}, {m.entries[3][0], m.entries[3][1], m.entries[3][3]} };
        float cofactor3 = m.entries[0][2] * getDet3by3(constructMatrix3By3(entries3));

        float entries4[3][3] = { {m.entries[1][0], m.entries[1][1], m.entries[1][2]}, {m.entries[2][0], m.entries[2][1], m.entries[2][2]}, {m.entries[3][0], m.entries[3][1], m.entries[3][2]} };
        float cofactor4 = m.entries[0][3] * getDet3by3(constructMatrix3By3(entries4));

        return cofactor1 - cofactor2 + cofactor3 - cofactor4;
    }

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
}
