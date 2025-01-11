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

QVector<Triangle * > MyPolygon::earClippingUtils(Vector2D * p1, Vector2D * p2, Vector2D *p3,
                                                QVector<Vector2D * > &vertices){

    //qDebug() << "earClippingUtils";
    QVector<Vector2D * > excluded;
    excluded.push_back(p1);
    excluded.push_back(p2);
    excluded.push_back(p3);

    Triangle *T =  new Triangle (p1,p2,p3);
    QVector<Triangle*> triangles;

    // Résultat après soustraction
    QVector<Vector2D * > result;

    for (auto &v : vertices) {
        bool isExcluded = false;

        // Comparer manuellement avec les éléments de excluded
        for (auto &e : excluded) {
            if (*v == *e) { // Assurez-vous que Vector2D surcharge l'opérateur `==`
                isExcluded = true;
                break;
            }
        }

        if (!isExcluded) {
            result.append(v);
        }
    }


    for(auto &v : result){

        if(T->isInside(v)){
            qDebug() << "TRIANGLE";
            qDebug() << *T->getVertexPtr(0);
            qDebug() << *T->getVertexPtr(1);
            qDebug() << *T->getVertexPtr(2);
            qDebug() << "POINT A LINTERIEUR : " << *v;

            triangles.append(this->earClippingUtils(p1,p2,v,result));
            triangles.append(this->earClippingUtils(p2,p3,v,result));
            triangles.append(this->earClippingUtils(p3,p1,v,result));

            qDebug() << "HIHI";

        }
    }

    if(triangles.size() == 0){
        //qDebug() << "HEEEEEEERE";
        triangles.push_back(T);

    }else{
        delete T;
    }
    return triangles;
}

QVector<Triangle*> MyPolygon::earClipping( QVector<Vector2D * > &vertices){
    QVector<Triangle*> triangles;
    //QVector<Vector2D*> tmp;


    if (N < 3) {
        qDebug() << "Pas assez de points pour former des triangles.\n";
    }


    // Génération des triangles (E0, Ei, Ei+1) pour i dans [1, m-2]
    qDebug() << "GROS BORDEL ICI";
            QVector<Vector2D * > excluded ;
    triangles = this->earClippingUtils (&tabPts[0],&tabPts[1],&tabPts[2],vertices);
    /*for (int i = 1; i < N - 1; ++i) {
        QVector<Vector2D * > excluded ;
        qDebug() << "ICI";
        triangles.append(this->earClippingUtils (&tabPts[0],&tabPts[i],&tabPts[i+1],vertices,excluded));


    }*/
    qDebug() << triangles.size();
    qDebug() << "LE BORDEL A PRIS FIN";
    for(auto &t : triangles){
       /* qDebug() << "TRi";
        qDebug() << *t->getVertexPtr(0);
        qDebug() << *t->getVertexPtr(1);
        qDebug() << *t->getVertexPtr(2);*/

    }
/*
    qDebug() << "BOUCLE DE EARCLIPPING";
    for(auto &v : vertices){
        tmp.push_back(v);
        qDebug() << *v;
    }

    tmp.push_back(vertices[0]);
    tmp.push_back(vertices[1]);

    int i=0;
    do {

        Triangle* T = new Triangle (tmp[i],tmp[i+1],tmp[i+2]);
        int j = i+3;
        while(j<tmp.size()-2 && !T->isInside(*tmp[j])){
            j++;
        }
        if(j==tmp.size()-2){
            triangles.push_back(T);
            tmp.removeAt(i+1);
            i = 0;
        }else{
            i++;
        }
    }while (tmp.size()>=5);*/
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


