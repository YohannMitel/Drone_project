#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <triangle.h>
#include "cities.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();

    void loadMesh(const QString &title);

    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent*) override;
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

    QPair<Vector2D,Vector2D> getBox();

    QVector<Triangle*> triangles;
;
    Cities *cities = new Cities();

    float scale=0.5;
    QPoint origin=QPoint(10,10);
public:
    bool flipped = false ;
    bool showTriangles=true,showCenters=false,showCircles=false, voronoiTransparency = false;
};

#endif // CANVAS_H
