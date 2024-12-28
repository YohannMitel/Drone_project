#include "mypolygon.h"
#include <QDebug>
#include <QString>
#include "nearestEdgeResult.h"
void MyPolygon::addVertex(const Vector2D &P) {
    assert(N<Nmax);
    tabPts[N].x=P.x;
    tabPts[N].y=P.y;
    N++;
    tabPts[N].x=tabPts[0].x;
    tabPts[N].y=tabPts[0].y;
}

void MyPolygon::draw(QPainter &painter) {
    QPen pen(Qt::black);
    pen.setWidth(3);

    QPoint *points=new QPoint[N];
    for (int i=0; i<N; i++) {

        points[i].setX(tabPts[i].x);
        points[i].setY(tabPts[i].y);
    }

    painter.setBrush(Qt::yellow);
    painter.setPen(pen);
    painter.drawPolygon(points,N,Qt::OddEvenFill);


    delete [] points;
}

bool MyPolygon::isOnTheLeft(const Vector2D &P,int i) {
    Vector2D AB = tabPts[i+1]-tabPts[i],
        AP = P-tabPts[i];
    return (AB.x*AP.y - AB.y*AP.x)>=0;
}

bool MyPolygon::isConvex() {
    int i=0;
    while (i<N && isOnTheLeft(tabPts[(i+2)%N],i)) {
        i++;
    }
    return (i==N);
}

bool MyPolygon::isInside(const Vector2D &P) {
    int i=0;
    while (i<N && isOnTheLeft(P,i)) {
        i++;
    }
    return (i==N);
}



NearestEdgeResult MyPolygon::nearestEdge(const Vector2D &M){
    double distance = 1000;
    int nearest = -1;


    for(int i = 0 ; i < N ; i++){
        double d = distanceToEdge(M, i );
        //qDebug() << QString::number(N) << distanceToEdge(M, i );
        if (distance > d){
            distance =  d;
            nearest = i;
        }
    }
    return NearestEdgeResult{distance,nearest};
}



double MyPolygon::distanceToEdge(const Vector2D &M, int i) {
    Vector2D AB = tabPts[i+1] - tabPts[i];  // Vector AB representing the edge
    Vector2D AM = M - tabPts[i];  // Vector AM from the edge start to point M
    double lAB = AB.length();  // Length of edge AB
    double AH = (AB.x * AM.x + AB.y * AM.y) / lAB;  // Projection of AM onto AB

    if (AH < 0) {  // The perpendicular projection falls before the start of the edge
        return AM.length();
    } else if (AH > lAB) {  // The perpendicular projection falls beyond the end of the edge
        return (M - tabPts[i+1]).length();
    } else {  // The perpendicular projection falls within the edge
        return abs(AB.x * AM.y - AB.y * AM.x) / lAB;
    }
}


