/**
 * @brief Drone_demo project
 * @author B.Piranda
 * @date dec. 2024
 **/
#ifndef CANVAS_H
#define CANVAS_H

#include <triangle.h>
#include "cities.h"
#include <QWidget>
#include <drone.h>
#include <QMouseEvent>
#include <QPaintEvent>

class Canvas : public QWidget {
    Q_OBJECT
public:
    const int droneIconSize=64; ///< size of the drone picture in the vanvas
    const double droneCollisionDistance=droneIconSize*1.5; ///< distance to detect collision with other drone
    /**
     * @brief Canvas constructor
     * @param parent
     */
    explicit Canvas(QWidget *parent = nullptr);
    /**
     * @brief setMap set the list of drones (identified by their name) to the canvas
     * @param map the map of couple "name of the drone"/"drone pointer"
     */

    ~Canvas();
    inline QVector<Drone*>& getMap() { return mapDrones; }
    /**
     * @brief paintEvent
     */
    void paintEvent(QPaintEvent*) override;
    /**
     * @brief mousePressEvent
     * @param event
     */
    void mousePressEvent(QMouseEvent *event) override;



    void loadMesh(const QString &title);


    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent*) override;

    void addPoints(QString &name ,const QVector<Vector2D> &tab);
    void addTriangle(int id0,int id1,int id2);
    void addTriangle(int id0,int id1,int id2,const QColor &color);
    void clear();
    void reScale();
    inline int getSizeofV() { return cities->getSize();}
    inline int getSizeofT() { return cities->getSize();}
    bool checkDelaunay();
    void flippAll();
    void processVoronoi(City &city);
    bool isOutsideCanvas(const Vector2D &point) const;
    void finalizePolygon(City &city, const QVector<Vector2D> &Lordered, bool isClosed);

    void processPoly();
    QVector<const Vector2D*> findOppositePointOfTrianglesWithEdgeCommon(const Triangle &tri);

    Vector2D calculateIntersection(const Vector2D &p1, const Vector2D &p2, float minX, float minY, float maxX, float maxY);

signals:

    /*
     * Update the status bar string with the parameter
     */
    void updateSB(QString s);
private:
    QVector<Drone*> mapDrones; ///< pointer on the map of the drones
    QImage droneImg; ///< picture representing the drone in the canvas
    QPair<Vector2D,Vector2D> getBox();

    QVector<Triangle*> triangles;

    Cities *cities = new Cities();

    float scale=0.5;
    QPoint origin=QPoint(10,10);
public:
    bool flipped = false ;
    bool showTriangles=true,showCenters=false,showCircles=false, voronoiTransparency = false;

};

#endif // CANVAS_H
