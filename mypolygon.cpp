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

void MyPolygon::draw(QPainter &painter, bool transparency) {
    QPen pen(Qt::black);
    pen.setWidth(3);

    QPoint *points=new QPoint[N];
    for (int i=0; i<N; i++) {

        points[i].setX(tabPts[i].x);
        points[i].setY(tabPts[i].y);
    }

    if(transparency){
        painter.setBrush(QColor (255,255,0,128));
    }else{
        painter.setBrush(Qt::yellow);
    }

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


QVector<Triangle*> MyPolygon::earClipping( QVector<Vector2D * > &vertices){
    QVector<Triangle*> triangles;
    QVector<Vector2D*> tmp;

    for(auto &v : vertices){
        tmp.push_back(v);
    }

    tmp.push_back(vertices[0]);
    tmp.push_back(vertices[1]);

    qDebug() << tmp;
    int i=0;
    do {

        Triangle* T = new Triangle (tmp[i],tmp[i+1],tmp[i+2]);
        int j = i+3;
        while(j<tmp.size()-2 && !T->isInside(*tmp[j])){
            j++;
        }
        if(j==tmp.size()-2){
            qDebug() << "A MIMIR";

            triangles.push_back(T);
            tmp.removeAt(i+1);
            i = 0;
        }else{
            i++;
        }
    }while (tmp.size()>=5);
    return triangles;
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


