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

void MyPolygon::draw(QPainter &painter, bool transparency, QString color) {
    QPen pen(Qt::black);
    pen.setWidth(3);

    QPoint *points=new QPoint[N];
    for (int i=0; i<N; i++) {

        points[i].setX(tabPts[i].x);
        points[i].setY(tabPts[i].y);
    }

    if(transparency){

        QColor transparentColor(color);
        transparentColor.setAlpha(128); // Réduire l'opacité à 50% (128/255)

        painter.setBrush(transparentColor);


    }else{
        painter.setBrush(QColor (color));
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
/* RECURSIVE FONCTION TO EARCLIP TRIANGLES */
QVector<Triangle *> MyPolygon::earClippingUtils(Vector2D *p1, Vector2D *p2, Vector2D *p3,
                                                QVector<Vector2D *> &vertices) {
    // Liste des points exclus (les sommets du triangle courant)
    QVector<Vector2D *> excluded = {p1, p2, p3};

    // Créer le triangle courant
    Triangle *T = new Triangle(p1, p2, p3);
    QVector<Triangle *> triangles;

    // Créer une liste des points restants après exclusion
    QVector<Vector2D *> result;

    for (auto &v : vertices) {
        // Vérification si v est dans excluded
        bool isExcluded = std::any_of(excluded.begin(), excluded.end(), [&](Vector2D *e) {
            return *v == *e; // Nécessite que Vector2D surcharge `==`
        });

        if (!isExcluded) {
            result.append(v);
        }
    }

    // Vérifier si des points sont à l'intérieur du triangle courant
    bool foundInteriorPoint = false;

    for (auto &v : result) {
        if (T->isInside(v)) {
            // Points trouvés à l'intérieur du triangle, appeler récursivement
            triangles += this->earClippingUtils(p1, p2, v, result);
            triangles += this->earClippingUtils(p2, p3, v, result);
            triangles += this->earClippingUtils(p3, p1, v, result);

            // Debugging (facultatif, pour voir le processus)
            /*qDebug() << "TRIANGLE";
            qDebug() << *T->getVertexPtr(0);
            qDebug() << *T->getVertexPtr(1);
            qDebug() << *T->getVertexPtr(2);
            qDebug() << "POINT À L'INTÉRIEUR : " << *v;*/

            foundInteriorPoint = true;
            break; // Si un point est trouvé à l'intérieur, on arrête la recherche
        }
    }

    if (!foundInteriorPoint) {
        // Aucun point à l'intérieur, conserver ce triangle
        triangles.push_back(T);
    } else {
        // Sinon, supprimer le triangle courant
        delete T;
    }

    return triangles;
}


QVector<Triangle*> MyPolygon::earClipping( QVector<Vector2D * > &vertices){
    QVector<Triangle*> triangles;

    // Génération des triangles (E0, Ei, Ei+1) pour i dans [1, m-2]


    for (int i = 1; i < N - 1; ++i) {
        QVector<Vector2D * > excluded ;
        triangles.append(this->earClippingUtils (&tabPts[0],&tabPts[i],&tabPts[i+1],vertices));

    }

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


