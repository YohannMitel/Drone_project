#ifndef VORONOI_H
#define VORONOI_H
#include <mypolygon.h>
#include <QVector>


class Voronoi {
private:
    QVector<MyPolygon * > tabPolygon;

public:
    Voronoi();
    ~Voronoi();


    void addPolygon(MyPolygon *p){
        tabPolygon.push_back(p);
    }

    void draw(QPainter &painter, bool transparency);


};
#endif // VORONOI_H
