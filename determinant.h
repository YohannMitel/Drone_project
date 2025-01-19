//
// Created by bpiranda on 22/11/2019.
//
/**
 * @brief Classes for handling 2x2, 3x3, and 4x4 matrices and calculating their determinants.
 */


#ifndef GEOMETRY_TRIANGULATION_GL_DETERMINANT_H
#define GEOMETRY_TRIANGULATION_GL_DETERMINANT_H

class Matrix33;
class Matrix44;

/**
 * @brief The Matrix22 class represents a 2x2 matrix and provides functionality to compute its determinant.
 */
class Matrix22 {
public:
    float m[2][2]; ///< Elements of the 2x2 matrix.

    /**
     * @brief Extracts a 2x2 submatrix from a 3x3 matrix by shadowing a specific row and column.
     * @param mat33 Reference to the source 3x3 matrix.
     * @param shadowLin Index of the row to shadow.
     * @param shadowCol Index of the column to shadow.
     */
    void get2x2From3x3(const Matrix33 &mat33, int shadowLin, int shadowCol);

    /**
     * @brief Computes the determinant of the 2x2 matrix.
     * @return Determinant of the matrix.
     */
    inline float determinant() {
        return m[0][0] * m[1][1] - m[0][1] * m[1][0];
    }
};

/**
 * @brief The Matrix33 class represents a 3x3 matrix and provides functionality to compute its determinant.
 */
class Matrix33 {
public:
    float m[3][3]; ///< Elements of the 3x3 matrix.

    /**
     * @brief Extracts a 3x3 submatrix from a 4x4 matrix by shadowing a specific row and column.
     * @param mat44 Reference to the source 4x4 matrix.
     * @param shadowLin Index of the row to shadow.
     * @param shadowCol Index of the column to shadow.
     */
    void get3x3From4x4(const Matrix44 &mat44, int shadowLin, int shadowCol);

    /**
     * @brief Computes the determinant of the 3x3 matrix.
     * @return Determinant of the matrix.
     */
    float determinant();
};

/**
 * @brief The Matrix44 class represents a 4x4 matrix and provides functionality to compute its determinant.
 */
class Matrix44 {
public:
    float m[4][4]; ///< Elements of the 4x4 matrix.

    /**
     * @brief Computes the determinant of the 4x4 matrix.
     * @return Determinant of the matrix.
     */
    float determinant();
};

#endif // GEOMETRY_TRIANGULATION_GL_DETERMINANT_H
