/**
 * @brief Drone_demo project
 * @author B.Piranda
 * @date Dec. 2024
 */
#ifndef CANVAS_H
#define CANVAS_H

#include <triangle.h>
#include "cities.h"
#include <QWidget>
#include <drone.h>
#include <QMouseEvent>
#include <QPaintEvent>

/**
 * @brief The Canvas class represents the drawing area for drones, cities, and other elements.
 */
class Canvas : public QWidget {
    Q_OBJECT
public:
    const int droneIconSize = 64; ///< Size of the drone icon in the canvas.
    const double droneCollisionDistance = droneIconSize * 1.5; ///< Distance to detect collision with another drone.

    /**
     * @brief Constructor for the Canvas class.
     * @param parent Pointer to the parent widget.
     */
    explicit Canvas(QWidget *parent = nullptr);

    /**
     * @brief Destructor for the Canvas class.
     */
    ~Canvas();

    /**
     * @brief Gets the list of drones.
     * @return Reference to the vector containing drone pointers.
     */
    inline QVector<Drone*>& getMap() { return mapDrones; }

    /**
     * @brief Gets the state of the polygon processing flag.
     * @return True if polygon processing is enabled, otherwise false.
     */
    inline bool getProcessPolyState() { return processPolyState; }

    /**
     * @brief Handles the paint event for the canvas.
     * @param event Pointer to the paint event.
     */
    void paintEvent(QPaintEvent* event) override;

    /**
     * @brief Handles mouse press events.
     * @param event Pointer to the mouse event.
     */
    void mousePressEvent(QMouseEvent* event) override;

    /**
     * @brief Loads a mesh file.
     * @param title The title or filename of the mesh.
     */
    void loadMesh(const QString &title);

    /**
     * @brief Handles resize events.
     * @param event Pointer to the resize event.
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief Handles mouse move events.
     * @param event Pointer to the mouse event.
     */
    void mouseMoveEvent(QMouseEvent* event) override;

    /**
     * @brief Adds a set of points to the canvas.
     * @param name Name associated with the points.
     * @param tab Vector containing the points.
     */
    void addPoints(QString &name, const QVector<Vector2D> &tab);

    /**
     * @brief Adds a triangle defined by three vertex indices.
     * @param id0 Index of the first vertex.
     * @param id1 Index of the second vertex.
     * @param id2 Index of the third vertex.
     */
    void addTriangle(int id0, int id1, int id2);

    /**
     * @brief Adds a triangle with a specified color.
     * @param id0 Index of the first vertex.
     * @param id1 Index of the second vertex.
     * @param id2 Index of the third vertex.
     * @param color Color of the triangle.
     */
    void addTriangle(int id0, int id1, int id2, const QColor &color);

    /**
     * @brief Clears all elements from the canvas.
     */
    void clear();

    /**
     * @brief Rescales the canvas.
     */
    void reScale();

    /**
     * @brief Gets the size of the vertices list.
     * @return The number of vertices.
     */
    inline int getSizeofV() { return cities->getSize(); }

    /**
     * @brief Gets the size of the triangles list.
     * @return The number of triangles.
     */
    inline int getSizeofT() { return cities->getSize(); }

    /**
     * @brief Checks if the Delaunay condition is satisfied for all triangles.
     * @return True if the condition is satisfied, otherwise false.
     */
    bool checkDelaunay();

    /**
     * @brief Flips all non-Delaunay edges.
     */
    void flippAll();

    /**
     * @brief Processes the Voronoi diagram for a given city.
     * @param city The city for which the Voronoi diagram is computed.
     */
    void processVoronoi(City &city);

    /**
     * @brief Checks if a point is outside the canvas boundaries.
     * @param point The point to check.
     * @return True if the point is outside, otherwise false.
     */
    bool isOutsideCanvas(const Vector2D &point) const;

    /**
     * @brief Finalizes a polygon for a given city.
     * @param city The city associated with the polygon.
     * @param Lordered The ordered list of points defining the polygon.
     * @param isClosed Flag indicating if the polygon is closed.
     */
    void finalizePolygon(City &city, const QVector<Vector2D> &Lordered, bool isClosed);

    /**
     * @brief Processes a polygon for rendering or analysis.
     */
    void processPoly();

    /**
     * @brief Finds the opposite points of triangles sharing a common edge.
     * @param tri The reference triangle.
     * @return Vector of pointers to the opposite points.
     */
    QVector<const Vector2D*> findOppositePointOfTrianglesWithEdgeCommon(const Triangle &tri);

    /**
     * @brief Calculates the intersection of a line with a bounding box.
     * @param p1 The starting point of the line.
     * @param p2 The ending point of the line.
     * @param minX Minimum x-coordinate of the bounding box.
     * @param minY Minimum y-coordinate of the bounding box.
     * @param maxX Maximum x-coordinate of the bounding box.
     * @param maxY Maximum y-coordinate of the bounding box.
     * @return The intersection point.
     */
    Vector2D calculateIntersection(const Vector2D &p1, const Vector2D &p2, float minX, float minY, float maxX, float maxY);

    /**
     * @brief Executes a sequence of actions for a given drone.
     * @param d The drone to process.
     */
    void droneSequence(Drone &d);

signals:

    /**
     * @brief Updates the status bar with a given message.
     * @param s The message to display.
     */
    void updateSB(QString s);

private:
    QVector<Drone*> mapDrones; ///< Vector containing pointers to drones.
    QImage droneImg; ///< Image representing a drone on the canvas.

    /**
     * @brief Calculates the bounding box.
     * @return A pair of points representing the bounding box.
     */
    QPair<Vector2D, Vector2D> getBox();

    QVector<Triangle*> triangles; ///< Vector containing pointers to triangles.
    bool processPolyState = false; ///< Flag indicating if polygon processing is enabled.
    Cities *cities = new Cities(); ///< Pointer to the cities object.

    float scale = 0.5; ///< Scaling factor for the canvas.
    QPoint origin = QPoint(10, 10); ///< Origin point of the canvas.

public:
    bool flipped = false; ///< Flag indicating if the canvas is flipped.
    bool showTriangles = true; ///< Flag to show or hide triangles.
    bool showCenters = false; ///< Flag to show or hide triangle centers.
    bool showCircles = false; ///< Flag to show or hide circles.
    bool voronoiTransparency = false; ///< Flag to enable or disable Voronoi transparency.
};

#endif // CANVAS_H
