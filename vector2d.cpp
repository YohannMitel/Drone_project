#include "vector2d.h"




Vector2D Vector2D::projection(const Vector2D &A, const Vector2D &B, const Vector2D &M) {
    Vector2D AB = B - A;   // Vector AB
    Vector2D AM = M - A;   // Vector AM

    // Projection factor t
    float t = AM.dot(AB) / (AB.length() * AB.length());

    // Clamp t to the range [0, 1]
    t = std::max(0.0f, std::min(1.0f, t));

    // Point M = A + t * AB
    return A + AB * t;
}

QString Vector2D::whichSide(const Vector2D &P, float canvasWidth, float canvasHeight) {
    //qDebug() << "P.x : " << P.x << "P.y : " << P.y;
    if (P.x <= 0) {
        qDebug() << "Point is to the left of the canvas.";
        return "left";
    }
    else if (P.x > canvasWidth-50) {
        qDebug() << "Point is to the right of the canvas.";
        return "right";
    }
    else if (P.y > canvasHeight ) {
        qDebug() << "Point is above the canvas.";
        return "above";
    }
    else {
        qDebug() << "Point is below the canvas.";
        return "bottom";
    }
}


Vector2D Vector2D::extendLineToCanvas(const Vector2D& M, const Vector2D& P, float canvasWidth, float canvasHeight) {
    // Ajouter une marge de +30 pixels à la hauteur et -30 pixels à la largeur
    float marginHeight = canvasHeight +30;
    float marginWidth = canvasWidth ;

    // Direction de la droite MP
    Vector2D MP = P - M;

    // Si la direction est nulle (M et P sont identiques), retourner M
    if (MP.x == 0 && MP.y == 0) {
        return M;
    }

    // Calcul des distances entre M et les bords du canvas
    float distLeft = P.x;                // Distance au bord gauche
    float distRight = marginWidth - P.x; // Distance au bord droit
    float distTop = marginHeight - P.y;  // Distance au bord supérieur
    float distBottom = P.y;              // Distance au bord inférieur

    // Déterminer le côté du canvas le plus proche de M
    float minDist = std::min({distLeft, distRight, distTop, distBottom});
    int side = 0; // 0 = gauche, 1 = droite, 2 = haut, 3 = bas

    if (minDist == distLeft) side = 0; // Bord gauche
    else if (minDist == distRight) side = 1; // Bord droit
    else if (minDist == distTop) side = 2; // Bord supérieur
    else if (minDist == distBottom) side = 3; // Bord inférieur

    // Calcul des intersections avec le côté sélectionné
    Vector2D intersection;
    if (side == 0) { // Bord gauche (x = 0)
        float t = (0 - M.x) / MP.x;
        intersection = M + MP * t;
    } else if (side == 1) { // Bord droit (x = marginWidth)
        float t = (marginWidth - M.x) / MP.x;
        intersection = M + MP * t;
    } else if (side == 2) { // Bord supérieur (y = marginHeight)
        float t = (marginHeight - M.y) / MP.y;
        intersection = M + MP * t;
    } else if (side == 3) { // Bord inférieur (y = 0)
        float t = (0 - M.y) / MP.y;
        intersection = M + MP * t;
    }

    // Retourner le point d'intersection
    return intersection;
}

Vector2D Vector2D::getCanvasIntersectionLimit(const Vector2D &A, const Vector2D &B, const Vector2D &M, float canvasWidth, float canvasHeight){
    const Vector2D proj = Vector2D::projection(A, B, M);
    const Vector2D inter = Vector2D::extendLineToCanvas(M, proj, canvasWidth, canvasHeight);

    return inter;
}


bool Vector2D::polarComparison(QPair<QString,Vector2D> P1,QPair<QString,Vector2D> P2) {

    auto pts1 = P1.second;
    auto pts2 = P2.second;


    // Priorité si x == 0
    if (pts1.x == 0 && pts2.x != 0) {
        return true; // P1 est prioritaire
    }
    if (pts2.x == 0 && pts1.x != 0) {
        return false; // P2 est prioritaire
    }

    // Calcul des angles polaires
    double a1 = asin(pts1.y / sqrt(pts1.x * pts1.x + pts1.y * pts1.y));
    if (pts1.x < 0.0) {
        a1 = M_PI - a1;
    }

    double a2 = asin(pts2.y / sqrt(pts2.x * pts2.x + pts2.y * pts2.y));
    if (pts2.x < 0.0) {
        a2 = M_PI - a2;
    }

    return a1<a2;
}

const Vector2D operator *(double a,const Vector2D &v) {
    return Vector2D(a*v.x,a*v.y);
}

 double operator *(const Vector2D &u,const Vector2D &v) {
    return u.x*v.x+u.y*v.y;
}

const Vector2D operator +(const Vector2D &u,const Vector2D &v) {
    return Vector2D(u.x+v.x,u.y+v.y);
}

const Vector2D operator -(const Vector2D &u,const Vector2D &v) {
    return Vector2D(u.x-v.x,u.y-v.y);
}

const Vector2D operator-(const Vector2D &v) {
    return Vector2D(-v.x,-v.y);
}

 double operator ^(const Vector2D &u,const Vector2D &v) {
    return u.x*v.y-u.y*v.x;
}

 bool operator==(const Vector2D &u,const Vector2D &v) {
    return (u.x==v.x && u.y==v.y);
}
bool edgesMatch(const Vector2D* p1, const Vector2D* p2, const Vector2D* p3, const Vector2D* p4) {
     // Compare the edges (p1, p2) and (p3, p4)
     return ((p1 == p3 && p2 == p4) || (p1 == p4 && p2 == p3));
}


