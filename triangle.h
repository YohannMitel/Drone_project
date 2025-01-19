#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QPainter>
#include <vector2d.h>
#include "determinant.h"
#include <QDebug>
#include <QVector>

class Triangle {
private:
    Vector2D *ptr[3]; ///< array of 3 pointers on the vertices
    Vector2D circumCenter; ///< the center of the triangle (by computeCenter)
    float circumRadius; ///< the radius of the circumCircle (by computeCenter)
    QBrush brush; ///< current brush to draw the triangle
    bool isHighlited=false; ///< is drawn highlighted
    bool isDelaunay=false; ///< is drawn as Delaunay triangle
    bool flippable=false; ///< is drawn as flippable triangle
    Vector2D *flippPoint = nullptr; ///< Point making this triangle flippable
    /**
     * @brief Computes the circumcircle of the triangle from its vertices.
     *
     * This function calculates the center and radius of the circumcircle
     * based on the coordinates of the triangle's vertices.
     */
    void computeCircle();


public:
    /**
     * @brief Construct a Triangle object with specified vertices.
     *
     * This constructor initializes the triangle with three vertices.
     * It also sets the color to yellow and computes the circumcircle.
     *
     * @param ptr1 Pointer to the first vertex.
     * @param ptr2 Pointer to the second vertex.
     * @param ptr3 Pointer to the third vertex.
     */
    Triangle(Vector2D *ptr1,Vector2D *ptr2,Vector2D *ptr3) {
        /*qDebug() << "PTR1" << *ptr1;
        qDebug() << "PTR2" << *ptr2;
        qDebug() << "PTR3" << *ptr3;*/
        ptr[0]=ptr1;
        ptr[1]=ptr2;
        ptr[2]=ptr3;
        setColor(Qt::yellow);
        computeCircle();
    }

    /**
     * @brief Construct a Triangle object with specified vertices and color.
     *
     * This constructor initializes the triangle with three vertices and
     * sets the specified color for the triangle. It also computes the
     * circumcircle.
     *
     * @param ptr1 Pointer to the first vertex.
     * @param ptr2 Pointer to the second vertex.
     * @param ptr3 Pointer to the third vertex.
     * @param p_color The color to set for the triangle.
     */
    Triangle(Vector2D *ptr1,Vector2D *ptr2,Vector2D *ptr3,const QColor &p_color) {
        ptr[0]=ptr1;
        ptr[1]=ptr2;
        ptr[2]=ptr3;
        setColor(p_color);
        computeCircle();
    }

    /**
     * @brief Check if a point is on the left side of the line segment P1P2.
     *
     * This function uses the cross-product method to determine if point P
     * lies on the left of the directed line from P1 to P2.
     *
     * @param P Pointer to the point to check.
     * @param P1 Pointer to the first endpoint of the line segment.
     * @param P2 Pointer to the second endpoint of the line segment.
     * @return True if point P is on the left side of P1P2, otherwise false.
     */
    bool isOnTheLeft(const Vector2D *P,const Vector2D *P1,const Vector2D *P2) {
        Vector2D AB = *P2-*P1,
            AP = *P-*P1;

        return (AB.x*AP.y - AB.y*AP.x)>=0;
    }

    /**
     * @brief Check if a point (x, y) is inside the triangle.
     *
     * This function checks if the point with coordinates (x, y) lies
     * inside the triangle.
     *
     * @param x The x-coordinate of the point.
     * @param y The y-coordinate of the point.
     * @return True if the point is inside the triangle, otherwise false.
     */
    inline bool isInside(float x,float y) {
        return isInside(Vector2D(x,y));
    }

    /**
     * @brief Check if a point P is inside the triangle.
     *
     * This function checks if the given point P lies inside the triangle.
     *
     * @param P The point to check.
     * @return True if the point is inside the triangle, otherwise false.
     */
    bool isInside(const Vector2D &P) {
        return isOnTheLeft(&P,ptr[0],ptr[1]) &&
               isOnTheLeft(&P,ptr[1],ptr[2]) &&
               isOnTheLeft(&P,ptr[2],ptr[0]);
    }

    /**
     * @brief Draw the triangle using the given QPainter.
     *
     * This function uses a QPainter to draw the triangle on a widget.
     *
     * @param painter The QPainter used to draw the triangle.
     */
    void draw(QPainter &painter);

    /**
     * @brief Draw the triangle with transparency using the given QPainter.
     *
     * This function draws the triangle with transparency using the given
     * QPainter.
     *
     * @param painter The QPainter used to draw the triangle with transparency.
     */
    void transparencyDraw(QPainter &painter);

    /**
     * @brief Draw the circumcircle of the triangle using the given QPainter.
     *
     * This function draws the circumcircle of the triangle using the given
     * QPainter.
     *
     * @param painter The QPainter used to draw the circumcircle.
     */
    void drawCircle(QPainter &painter);

    /**
     * @brief Set the color of the triangle.
     *
     * This function sets the color of the triangle for rendering.
     *
     * @param p_color The color to set.
     */
    void setColor(const QColor &p_color) {
        brush.setStyle(Qt::BrushStyle::SolidPattern);
        brush.setColor(p_color);
    }

    /**
     * @brief Set the highlighted status of the triangle.
     *
     * This function sets whether the triangle is highlighted.
     *
     * @param v The value to set. True for highlighted, false for not highlighted.
     */
    inline void setHighlighted(bool v) {
        isHighlited=v;
    }

    /**
     * @brief Get the highlighted status of the triangle.
     *
     * This function returns whether the triangle is highlighted.
     *
     * @return True if the triangle is highlighted, otherwise false.
     */
    inline bool isHighlighted() {
        return isHighlited;
    }

    /**
     * @brief Check if the triangle is flippable.
     *
     * This function returns whether the triangle can be flipped in a
     * Delaunay triangulation.
     *
     * @return True if the triangle is flippable, otherwise false.
     */
    inline bool isFlippable() {
        return flippable;
    }

    /**
     * @brief Get the center of the circumcircle.
     *
     * This function returns the center of the circumcircle of the triangle.
     *
     * @return The center of the circumcircle.
     */
    inline Vector2D getCircleCenter(){
        return circumCenter;
    }

    /**
     * @brief Set the Delaunay status and flippable status of the triangle.
     *
     * This function sets whether the triangle is Delaunay-compliant and if it
     * is flippable.
     *
     * @param v True if the triangle is Delaunay-compliant, false otherwise.
     * @param f True if the triangle is flippable, false otherwise.
     */
    inline void setDelaunay(bool v, bool f) {
        isDelaunay=v;
        flippable=f;
    }

    /**
     * @brief Set the opposite point for the triangle to make it flippable.
     *
     * This function sets the opposite point that makes the triangle flippable
     * for Delaunay triangulation.
     *
     * @param o Pointer to the opposite vertex.
     */
    inline void setOpposite(Vector2D *o){
        flippPoint = o;
    }

    /**
     * @brief Get the opposite point that makes the triangle flippable.
     *
     * This function returns the opposite point that makes the triangle flippable.
     *
     * @return Pointer to the opposite vertex.
     */
    inline Vector2D* getOpposite(){
        return flippPoint;
    }

    /**
     * @brief Check if the triangle satisfies the Delaunay condition.
     *
     * This function checks if the triangle satisfies the Delaunay condition
     * in relation to all vertices in the given list.
     *
     * @param tabVertices The list of vertices to check.
     * @return True if the triangle is Delaunay-compliant, otherwise false.
     */
    bool checkDelaunay(const QVector<Vector2D *> &tabVertices) {
        auto it = tabVertices.begin();
        bool isOk = true;


        // Assume tabVertices has at least three points, A, B, and C
        // Assume tabVertices has at least three points, A, B, and C
        const Vector2D *A = ptr[0];
        const Vector2D *B = ptr[1];
        const Vector2D *C = ptr[2];

        while (it != tabVertices.end() && isOk) {
            Matrix33 mat;
            // PAGE 35 DU COURS GEOMETRIC ALGOITHMS
            const Vector2D D = (*it);
            mat.m[0][0] = A->x - D.x;
            mat.m[0][1] = A->y - D.y;
            mat.m[0][2] = ((A->x * A->x) - (D.x * D.x)) + ((A->y * A->y) - (D.y * D.y));

            mat.m[1][0] = B->x - D.x;
            mat.m[1][1] = B->y - D.y ;
            mat.m[1][2] = ((B->x * B->x) - (D.x * D.x)) + ((B->y * B->y) - (D.y * D.y));

            mat.m[2][0] =   C->x - D.x;
            mat.m[2][1] =  C->y - D.y ;
            mat.m[2][2] = ((C->x * C->x) - (D.x * D.x)) + ((C->y * C->y) - (D.y * D.y));

            isOk = (mat.determinant() <= 0);
            it++;
        };
        isDelaunay=isOk;
        flippable=false;
        //qDebug() << isDelaunay;
        return isDelaunay;


    }

    /**
     * @brief Get the pointer to the vertex at the specified index.
     *
     * This function returns the pointer to the vertex at the given index.
     *
     * @param i The index of the vertex (0, 1, or 2).
     * @return Pointer to the vertex at the given index.
     */
    Vector2D* getVertexPtr(int i) const{
        return ptr[i];
    }

    /**
     * @brief Check if the triangle contains the edge formed by points A and B.
     *
     * This function checks if the triangle contains the edge formed by the
     * points A and B.
     *
     * @param A The first point.
     * @param B The second point.
     * @return True if the edge is part of the triangle, otherwise false.
     */
    bool hasEdge(  Vector2D A, Vector2D B) const{
        return (A==ptr[0] && B==ptr[1]) || (A==ptr[1] && B==ptr[2]) || (A==ptr[2] && B==ptr[0]); //|| (A==ptr[1] && B==ptr[0]) || (A==ptr[2] && B==ptr[1]) || (A==ptr[0] && B==ptr[2]);

    }

    /**
     * @brief Get the vertex connected to point P by an edge.
     *
     * This function returns the vertex connected to point P by an edge of
     * the triangle.
     *
     * @param P The point to find the connected vertex for.
     * @return Pointer to the connected vertex.
     */
    Vector2D* getEdgeFrom(const Vector2D &P) const{
        if(ptr[0] == P) return ptr[1];
        if(ptr[1] == P) return ptr[2];
        if(ptr[2] == P) return ptr[0];
        return nullptr;
    }

    /**
     * @brief Get the vertex connected to point P by an edge.
     *
     * This function returns the vertex connected to point P by an edge of
     * the triangle.
     *
     * @param P The point to find the connected vertex for.
     * @return Pointer to the connected vertex.
     */
    Vector2D* getEdgeTo(const Vector2D &P) const{
        if(ptr[0] == P) return ptr[2];
        if(ptr[1] == P) return ptr[0];
        if(ptr[2] == P) return ptr[1];
        return nullptr;
    }

    /**
     * @brief Check if point M is one of the triangle's vertices.
     *
     * This function checks if the point M is one of the three vertices of
     * the triangle.
     *
     * @param M The point to check.
     * @return True if M is a vertex of the triangle, otherwise false.
     */
    bool contains(const Vector2D &M){
        return M==(*ptr[0]) ||   M==(*ptr[1]) ||  M==(*ptr[2]);
    }

    /**
     * @brief Update the vertices of the triangle.
     *
     * This function updates the three vertices of the triangle.
     *
     * @param _A Pointer to the new first vertex.
     * @param _B Pointer to the new second vertex.
     * @param _C Pointer to the new third vertex.
     */
    void updateVertices( Vector2D *_A,  Vector2D *_B,  Vector2D *_C){
        ptr[0] = _A;
        ptr[1] = _B;
        ptr[2] =_C;
    }

    /**
     * @brief Check if point M lies inside the circumcircle of the triangle.
     *
     * This function checks if the point M lies inside the circumcircle of
     * the triangle.
     *
     * @param M The point to check.
     * @return True if M lies inside the circumcircle, otherwise false.
     */
    bool circleContains(const Vector2D *M){
        Matrix33 mat;
        Vector2D *A = ptr[0];
        Vector2D *B = ptr[1];
        Vector2D *C = ptr[2];

        mat.m[0][0] = A->x - M->x;
        mat.m[0][1] = A->y - M->y;
        mat.m[0][2] = ((A->x * A->x) - (M->x * M->x)) + ((A->y * A->y) - (M->y * M->y));

        mat.m[1][0] = B->x - M->x;
        mat.m[1][1] = B->y - M->y ;
        mat.m[1][2] = ((B->x * B->x) - (M->x * M->x)) + ((B->y * B->y) - (M->y * M->y));

        mat.m[2][0] =   C->x - M->x;
        mat.m[2][1] =  C->y - M->y ;
        mat.m[2][2] = ((C->x * C->x) - (M->x * M->x)) + ((C->y * C->y) - (M->y * M->y));

        return mat.determinant()<=0;
    }

    /**
     * @brief Attempt to flip the triangle with one of its neighboring triangles.
     *
     * This function attempts to flip the current triangle with one of its
     * neighboring triangles, if possible.
     *
     * @param triangles The list of neighboring triangles to check.
     * @return True if the flip is successful, otherwise false.
     */
    bool flippIt(QVector<Triangle*> triangles);
};

#endif // TRIANGLE_H
