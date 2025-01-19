#ifndef MYPOLYGON_H
#define MYPOLYGON_H

#include <vector2d.h>
#include <QPainter>
#include <nearestEdgeResult.h>
#include "triangle.h"

/**
 * @brief The MyPolygon class allows the creation, drawing, and manipulation of polygons.
 * This class provides methods to check if a point is inside the polygon and compute its surface.
 */
class MyPolygon {
private:
    int Nmax; ///< Maximum number of vertices for the current polygon.
    int N; ///< Current number of vertices.
    QVector<Triangle> triangles; ///< Array of triangles representing the triangulation.
    Vector2D *tabPts; ///< Array of vertex positions.
    QString currentColor; ///< Current drawing color of the polygon.
    bool hovered = false; ///< Flag indicating if the polygon is hovered.

public:
    /**
     * @brief Constructor for the MyPolygon class.
     * @param p_Nmax Maximum number of vertices for the polygon.
     */
    MyPolygon(int p_Nmax) : Nmax(p_Nmax) {
        N = 0;
        tabPts = new Vector2D[Nmax + 1];
        currentColor = "#0000FF";
    }

    /**
     * @brief Destructor for the MyPolygon class.
     */
    ~MyPolygon() {
        delete[] tabPts;
    }

    /**
     * @brief Gets the vertices of the polygon.
     * @param n Reference to an integer that will store the number of vertices.
     * @return Pointer to the array of vertex positions.
     */
    Vector2D *getVertices(int &n) { n = N; return tabPts; };

    /**
     * @brief Adds a vertex to the polygon.
     * @param P The vertex position to add.
     */
    void addVertex(const Vector2D &P);

    /**
     * @brief Draws the polygon on a QPainter canvas.
     * @param painter Reference to the QPainter object.
     * @param transparency Flag to enable or disable transparency.
     */
    void draw(QPainter &painter, bool transparency);

    /**
     * @brief Checks if a point is inside the polygon using triangulation.
     * @param pt The point to check.
     * @return True if the point is inside, false otherwise.
     */
    bool isInsideWithTriangulation(const Vector2D &pt) {
        auto it = triangles.begin();
        while (it != triangles.end() && !(it->isInside(pt))) {
            it++;
        }
        hovered = (it == triangles.end() ? false : true);

        return hovered;
    }

    /**
     * @brief Sets the drawing color of the polygon.
     * @param c The new color in string format.
     */
    inline void setColor(const QString c) { currentColor = c; }

    /**
     * @brief Checks if a point is on the left side of a polygon edge.
     * @param P The point to check.
     * @param i The index of the edge.
     * @return True if the point is on the left, false otherwise.
     */
    bool isOnTheLeft(const Vector2D &P, int i);

    /**
     * @brief Checks if the polygon is convex.
     * @return True if the polygon is convex, false otherwise.
     */
    bool isConvex();

    /**
     * @brief Checks if a point is inside the polygon.
     * @param P The point to check.
     * @return True if the point is inside, false otherwise.
     */
    bool isInside(const Vector2D &P);

    /**
     * @brief Triangulates the polygon using the ear-clipping method.
     */
    void earClipping();

    /**
     * @brief Triangulates the polygon using the ear-clipping method and returns the resulting triangles.
     * @param vertices The vertices of the polygon.
     * @return A QVector of pointers to the resulting triangles.
     */
    QVector<Triangle*> earClipping(QVector<Vector2D *> &vertices);

    /**
     * @brief Computes the distance from a point to a specific edge of the polygon.
     * @param M The point to measure from.
     * @param i The index of the edge.
     * @return The distance to the edge.
     */
    double distanceToEdge(const Vector2D &M, int i);

    /**
     * @brief Finds the nearest edge of the polygon to a given point.
     * @param M The point to check.
     * @return A NearestEdgeResult object containing information about the nearest edge.
     */
    NearestEdgeResult nearestEdge(const Vector2D &M);

    /**
     * @brief Finds the closest vertex horizontally to a given point.
     * @param M The point to check.
     * @param nearest Reference to store the index of the nearest vertex.
     * @return The position of the closest vertex.
     */
    Vector2D closestH(const Vector2D &M, int &nearest);

    /**
     * @brief Helper function for ear-clipping triangulation.
     * @param p1 Pointer to the first vertex.
     * @param p2 Pointer to the second vertex.
     * @param p3 Pointer to the third vertex.
     * @param vertices The vertices of the polygon.
     * @return A QVector of pointers to the resulting triangles.
     */
    QVector<Triangle*> earClippingUtils(Vector2D *p1, Vector2D *p2, Vector2D *p3, QVector<Vector2D *> &vertices);
};

#endif // MYPOLYGON_H
