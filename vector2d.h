/**
 * @brief Drone_demo project
 * @author B.Piranda
 * @date dec. 2024
 **/
#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>
#include <QDebug>

/**
 * @class Vector2D
 * @brief Represents a 2D vector with various mathematical and geometric utilities.
 */

class Vector2D {
public:
    float x,y; ///< coordinates of the vector

    /**
     * @brief Constructs a vector with specified x and y coordinates.
     * @param p_x X component of the vector.
     * @param p_y Y component of the vector.
     */
    Vector2D(float p_x,float p_y):x(p_x),y(p_y) {};

    /**
     * @brief Default constructor initializing the vector to (0, 0).
     */
    Vector2D() { x=y=0.0; }

    /**
     * @brief Copy constructor to initialize from another vector.
     * @param p Pointer to the vector to copy.
     */
    Vector2D(Vector2D *p):x(p->x),y(p->y) {};

    /**
     * @brief set new coordinates to the vector
     * @param p_x: x component
     * @param p_y: y component
     */
    void set(float p_x,float p_y) { x=p_x; y=p_y; }

    /**
     * @brief Computes the length (or norm) of the vector.
     * @return The length of the vector.
     */
    double length() const {
        return sqrt(x*x+y*y);
    }

    /**
     * @brief Normalizes the vector to have a norm of 1 while maintaining its direction.
     */
    void normalize() {
        float l=float(length());
        x/=l;
        y/=l;
    }

    /**
     * @brief Returns a new vector that is orthogonal to this vector and normalized.
     * @return The orthonormal vector.
     */
    Vector2D orthoNormed() const {
        float l=float(length());
        return Vector2D(y/l,-x/l);
    }

    /**
     * @brief Accesses the components of the vector.
     * @param i Index (0 for x, 1 for y).
     * @return The x or y component of the vector.
     */
    float operator[](const int i) const {
        return (i==0)?x:y;
    };

    /**
     * @brief Adds another vector to this vector.
     * @param v The vector to add.
     */
    void operator+=(const Vector2D& v) {
        x+=v.x;
        y+=v.y;
    }


    friend double operator *(const Vector2D&,const Vector2D&);
    friend const Vector2D operator *(double,const Vector2D&);
    friend const Vector2D operator +(const Vector2D&,const Vector2D&);
    friend const Vector2D operator -(const Vector2D&,const Vector2D&);
    friend const Vector2D operator -(const Vector2D&);
    friend double operator ^(const Vector2D&,const Vector2D&);
    friend bool operator ==(const Vector2D&,const Vector2D&);
    friend bool operator !=(const Vector2D&,const Vector2D&);
    friend QDebug operator<<(QDebug dbg, const Vector2D& v) {
        dbg.nospace() << "Vector2D(" << v.x << ", " << v.y << ")";
        return dbg.space();
    }

    /**
     * @brief Checks if the vector is inside a specified rectangular canvas.
     * @param width Width of the canvas.
     * @param height Height of the canvas.
     * @return True if the vector is inside the canvas, false otherwise.
     */
    bool isInsideCanvas(float width, float height) const {
        return x >= 0 && x <= width && y >= 0 && y <= height;
    }

    /**
     * @brief Multiplies the vector by a scalar.
     * @param scalar Scalar value.
     * @return The scaled vector.
     */
    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    /**
     * @brief Computes the dot product of this vector with another vector.
     * @param op The other vector.
     * @return The dot product of the two vectors.
     */
    float dot(const Vector2D &op) {
        return x * op.x + y * op.y;
    }


    /**
     * @brief Computes the orthogonal projection of a point onto a line defined by two vectors.
     * @param A Starting point of the line.
     * @param B Ending point of the line.
     * @param M Point to project.
     * @return The projection point on the line.
     */
    static Vector2D projection(const Vector2D &A, const Vector2D &B, const Vector2D &M);


    /**
     * @brief Extends a line segment to the boundary of a canvas.
     * @param M Starting point of the line.
     * @param P Direction point of the line.
     * @param canvasWidth Width of the canvas.
     * @param canvasHeight Height of the canvas.
     * @return The intersection point on the canvas boundary.
     */
    static Vector2D extendLineToCanvas(const Vector2D& M, const Vector2D& P, float canvasWidth, float canvasHeight);


    /**
     * @brief Determines which side of the canvas a point lies on.
     * @param P Point to check.
     * @param canvasWidth Width of the canvas.
     * @param canvasHeight Height of the canvas.
     * @return A string indicating the side of the canvas ("top", "bottom", "left", "right").
     */
    static QString whichSide(const Vector2D &P, float canvasWidth, float canvasHeight);

    /**
     * @brief Compares two points based on their polar angle relative to the origin.
     * @param P1 First point pair.
     * @param P2 Second point pair.
     * @return True if P1 is less than P2 in polar angle, false otherwise.
     */
    static bool polarComparison(QPair<QString,Vector2D> P1,QPair<QString,Vector2D> P2);


    /**
     * @brief Computes the intersection point on the canvas boundary after projecting a point onto a vector.
     * @param A Starting point of the vector.
     * @param B Ending point of the vector.
     * @param M Point to project.
     * @param canvasWidth Width of the canvas.
     * @param canvasHeight Height of the canvas.
     * @return The intersection point on the canvas boundary.
     */
    static Vector2D getCanvasIntersectionLimit(const Vector2D &A, const Vector2D &B, const Vector2D &M, float canvasWidth, float canvasHeight) ;

    //////////////////////////////////


};

const Vector2D operator *(double,const Vector2D&);
double operator *(const Vector2D&,const Vector2D&);
const Vector2D operator +(const Vector2D&,const Vector2D&);
const Vector2D operator -(const Vector2D&,const Vector2D&);
const Vector2D operator -(const Vector2D&);
double operator ^(const Vector2D&,const Vector2D&);
bool operator==(const Vector2D&,const Vector2D&);
bool operator !=(const Vector2D&,const Vector2D&);
/**
     * @brief Checks if the edges defined by two pairs of points match.
     * @param p1 First point of the first edge.
     * @param p2 Second point of the first edge.
     * @param p3 First point of the second edge.
     * @param p4 Second point of the second edge.
     * @return True if the edges match, false otherwise.
     */
static bool edgesMatch(const Vector2D* p1, const Vector2D* p2, const Vector2D* p3, const Vector2D* p4);


#endif // VECTOR2D_H
