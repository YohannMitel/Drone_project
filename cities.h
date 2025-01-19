/**
 * @brief Cities management module for handling city data and geometric operations.
 * @author Yohann Mitel & Pierre Meyer
 * @date Janvier 2025
 **/
#ifndef CITIES_H
#define CITIES_H

#include "city.h"
#include "mypolygon.h"

/**
 * @brief The Cities class manages a collection of cities and associated geometric operations.
 */
class Cities {
private:
    QVector<City*> tabCities; ///< Vector containing pointers to city objects.
    MyPolygon *convexHull = nullptr; ///< Pointer to the convex hull of the cities.
    QVector<QVector<int>> adjacencyMatrix; ///< Adjacency matrix for city connections.
    float heightLimit; ///< Height limit for the map.
    float widthLimit; ///< Width limit for the map.

public:
    /**
     * @brief Constructor for the Cities class.
     */
    Cities();

    /**
     * @brief Destructor for the Cities class.
     */
    ~Cities();

    /**
     * @brief Gets the number of cities.
     * @return The size of the city vector.
     */
    qsizetype getSize() const;

    /**
     * @brief Clears all cities and related data.
     */
    void clear();

    /**
     * @brief Gets the position of a city by its index.
     * @param index Index of the city.
     * @return Pointer to the city's position.
     */
    Vector2D* getPointByIndex(int index);

    /**
     * @brief Adds a new city to the collection.
     * @param name Name of the city.
     * @param position Pointer to the city's position.
     * @param color Color associated with the city.
     */
    void pushCity(QString name, Vector2D *position, QString color);

    /**
     * @brief Orders points by ascending polar angle relative to a given origin.
     * @param porigin Reference to the origin point.
     * @return Vector of name and position pairs, sorted by polar angle.
     */
    QVector<QPair<QString, Vector2D>> ascendingPolarAngle(Vector2D &porigin);

    /**
     * @brief Checks if a point is on the left side of a line.
     * @param P The point to check.
     * @param top_1 First point of the line.
     * @param top Second point of the line.
     * @return True if the point is on the left, otherwise false.
     */
    bool isOnTheLeft(const Vector2D &P, const Vector2D &top_1, const Vector2D &top);

    /**
     * @brief Orders polygon points relative to a given origin.
     * @param porigin Reference to the origin point.
     */
    void orderPolygonPoint(Vector2D &porigin);

    /**
     * @brief Sorts cities based on their relative positions to a set of points.
     * @param city1 Pointer to the first city.
     * @param city2 Pointer to the second city.
     * @param pointsRelative Vector of name and position pairs.
     * @return True if city1 is less than city2, otherwise false.
     */
    static bool sortingByPointsRelative(const City* city1, const City* city2,
                                        const QVector<QPair<QString, Vector2D>>& pointsRelative);

    /**
     * @brief Gets the vector of city pointers.
     * @return Vector containing city pointers.
     */
    QVector<City*> getTabCities();

    /**
     * @brief Gets the vector of vertex pointers.
     * @return Vector containing vertex pointers.
     */
    QVector<Vector2D*> getTabVertices();

    /**
     * @brief Initializes the triangulation of cities.
     * @return Vector containing pointers to triangles.
     */
    QVector<Triangle*> initTriangulation();

    /**
     * @brief Builds the connection matrix for a set of cities.
     * @param cities Vector of city pointers.
     */
    void connectionMatrix(const QVector<City*>& cities);

    /**
     * @brief Checks if two polygons are neighbors based on their vertices.
     * @param vertices1 Pointer to the vertices of the first polygon.
     * @param numVertices1 Number of vertices in the first polygon.
     * @param vertices2 Pointer to the vertices of the second polygon.
     * @param numVertices2 Number of vertices in the second polygon.
     * @return True if the polygons are neighbors, otherwise false.
     */
    bool areNeighbors(Vector2D* vertices1, int numVertices1, Vector2D* vertices2, int numVertices2);

    /**
     * @brief Checks if two line segments intersect.
     * @param p1 Start point of the first segment.
     * @param p2 End point of the first segment.
     * @param q1 Start point of the second segment.
     * @param q2 End point of the second segment.
     * @return True if the segments intersect, otherwise false.
     */
    bool doIntersect(Vector2D p1, Vector2D p2, Vector2D q1, Vector2D q2);

    /**
     * @brief Determines the orientation of three points.
     * @param p First point.
     * @param q Second point.
     * @param r Third point.
     * @return 0 if collinear, 1 if clockwise, 2 if counterclockwise.
     */
    int orientation(Vector2D p, Vector2D q, Vector2D r);

    /**
     * @brief Finds the index of a city by its name.
     * @param name Name of the city.
     * @return Index of the city, or -1 if not found.
     */
    int getCityByName(QString &name);

    /**
     * @brief Draws the cities and connections on the canvas.
     * @param painter Reference to the QPainter object.
     * @param transparency Flag to enable or disable transparency.
     */
    void draw(QPainter &painter, bool transparency);

    /**
     * @brief Tests pathfinding between two cities.
     * @param start Index of the start city.
     * @param end Index of the end city.
     */
    void testPathFinding(int start, int end);

    /**
     * @brief Finds the next city destination along the path between two cities.
     * @param start Index of the start city.
     * @param end Index of the end city.
     * @return Position of the next destination city.
     */
    Vector2D nextDestCityId(int start, int end);

    /**
     * @brief Sets the width and height limits for the map.
     * @param width Reference to the width limit.
     * @param height Reference to the height limit.
     */
    void setLimit(int &width, int &height) {
        widthLimit = width;
        heightLimit = height;
    }

    /**
     * @brief Checks if a point is outside the cities' boundaries.
     * @param point The point to check.
     * @return True if the point is outside, otherwise false.
     */
    bool isOutsideCities(const Vector2D &point) const {
        return point.x < 0 || point.x > (widthLimit - 10) ||
               point.y < 0 || point.y > (heightLimit + 10);
    }
};

#endif // CITIES_H
