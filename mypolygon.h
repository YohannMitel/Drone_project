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
    QVector<Triangle> triangles; // array of triangles of the triangulation
    Vector2D *tabPts; // array of vertex positions
    QString currentColor; // current drawing color
    bool hovered = false;

public:
    MyPolygon(int p_Nmax):Nmax(p_Nmax) {
        N=0;
        tabPts = new Vector2D[Nmax+1];
        currentColor = "#0000FF";
    }
    ~MyPolygon() {
        delete [] tabPts;
    }


    Vector2D *getVertices(int &n) { n=N; return tabPts; };
    void addVertex(const Vector2D &P);
    void draw(QPainter &painter, bool transparency);
    void changeColor(const Vector2D &pt) {
        //isInside(pt) ? setColor(Qt::red) : setColor(currentColor) ;
        auto it=triangles.begin();
        while (it!=triangles.end() && !(it->isInside(pt))) {
            it++;
        }
        hovered= (it==triangles.end()?false:true);
/*
            qDebug() << *triangles[0].getVertexPtr(0);
                        qDebug() << *triangles[1].getVertexPtr(1);
                        qDebug() << *triangles[1].getVertexPtr(2);
                        qDebug() << triangles[1].isInside(pt);
            qDebug() << pt;
*/
           // qDebug() << currentColor;


        //currentColor =  (isInside(pt) ?Qt::red : Qt::red );
    }

    inline void setColor(const QString c) { currentColor=c; }


    bool isOnTheLeft(const Vector2D &P,int i);
    bool isConvex();
    bool isInside(const Vector2D &P);
    void earClipping();
    QVector<Triangle*> earClipping( QVector<Vector2D * > &vertices);

    double distanceToEdge(const Vector2D &M,int i);
    NearestEdgeResult nearestEdge(const Vector2D &M);
    Vector2D closestH(const Vector2D &M,int &nearest);

    QVector<Triangle * > earClippingUtils(Vector2D * p1, Vector2D * p2, Vector2D *p3,
                                         QVector<Vector2D * > &vertices);

};



#endif // MYPOLYGON_H
