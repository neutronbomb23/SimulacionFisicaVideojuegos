#pragma once
class Matrix4x4 {
public:
    float data[4][4];
    // Constructor por defecto
    Matrix4x4() {
        // Inicializa la matriz como la matriz identidad
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (i == j) {
                    m[i][j] = 1.0f;
                }
                else {
                    m[i][j] = 0.0f;
                }
            }
        }
    }


    Vector4 operator*(const Vector4& vec) const {
        Vector4 result;

        for (int i = 0; i < 4; ++i) {
            result[i] = 0.0f;
            for (int j = 0; j < 4; ++j) {
                result[i] += m[i][j] * vec[j];
            }
        }

        return result;
    }
    // Calcula la inversa de la matriz
    Matrix4x4 inverse() const {
        Matrix4x4 inv;

        float invDet = 1.0f / determinant();
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                float minorMat[3][3];
                int sign = ((i + j) % 2 == 0) ? 1 : -1;

                computeMinorMatrix(i, j, minorMat);
                float minorDet = minorMatrixDeterminant(minorMat);
                float cofactor = sign * minorDet * invDet;

                inv.data[j][i] = cofactor; // Asigna valores a la matriz inversa
            }
        }

        return inv;
    }

    float determinant() const {
        float det = 0.0f;

        // Regla de Sarrus para calcular el determinante
        for (int i = 0; i < 4; ++i) {
            det += data[0][i] * data[1][(i + 1) % 4] * data[2][(i + 2) % 4] * data[3][(i + 3) % 4];
            det -= data[0][i] * data[1][(i + 3) % 4] * data[2][(i + 2) % 4] * data[3][(i + 1) % 4];
        }

        return det;
    }

    float minorMatrixDeterminant(float mat[3][3]) const {
        float det = 0.0f;

        // Calcula el determinante de una matriz 3x3
        det = mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]) -
            mat[0][1] * (mat[1][0] * mat[2][2] - mat[2][0] * mat[1][2]) +
            mat[0][2] * (mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1]);

        return det;
    }

    void computeMinorMatrix(int row, int col, float minorMat[3][3]) const {
        int rowIdx = 0;
        for (int i = 0; i < 4; ++i) {
            if (i == row) continue;

            int colIdx = 0;
            for (int j = 0; j < 4; ++j) {
                if (j == col) continue;

                minorMat[rowIdx][colIdx] = data[i][j];
                ++colIdx;
            }
            ++rowIdx;
        }
    }
    // Getter para acceder a los valores de la matriz
    float get(int row, int col) const {
        return m[row][col];
    }

    // Setter para establecer valores en la matriz
    void set(int row, int col, float value) {
        m[row][col] = value;
    }

private:
    float m[4][4]; // Matriz 4x4
};

