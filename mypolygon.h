#ifndef MYPOLYGON_H
#define MYPOLYGON_H
#include <vector2d.h>
#include <QPainter>
#include <nearestEdgeResult.h>
#include "triangle.h"
/**
 * @brief The MyPolygon class allwo to create, draw and manipulate polygons,
 *  especially check if a point is inside and compute the surface of the polygon
 */

class MyPolygon
{
private:
    int Nmax; // maximum number of vertices for the current Polygon
    int N; // current number of Vertices
    Vector2D *tabPts; // array of vertex positions
    QColor currentColor; // current drawing color

public:
    MyPolygon(int p_Nmax):Nmax(p_Nmax) {
        N=0;
        tabPts = new Vector2D[Nmax+1];
        currentColor=Qt::green;
    }
    ~MyPolygon() {
        delete [] tabPts;
    }



    Vector2D *getVertices(int &n) { n=N; return tabPts; };
    void addVertex(const Vector2D &P);
    void draw(QPainter &painter, bool transparency);
    void setColor(const QColor c) { currentColor=c; }


    bool isOnTheLeft(const Vector2D &P,int i);
    bool isConvex();
    bool isInside(const Vector2D &P);
    QVector<Triangle*> earClipping( QVector<Vector2D * > &vertices);

    double distanceToEdge(const Vector2D &M,int i);
    NearestEdgeResult nearestEdge(const Vector2D &M);
    Vector2D closestH(const Vector2D &M,int &nearest);

};



#endif // MYPOLYGON_H
