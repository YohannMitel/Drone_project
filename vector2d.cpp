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


QString Vector2D::whichSide(const Vector2D &P, float canvasWidth, float canvasHeight){
    if (P.x <= 0) {
        qDebug() << "Point is to the left of the canvas.";
        return "left";
    } else if (P.x  >= canvasWidth) {
        qDebug() << "Point is to the right of the canvas.";
        return "right";
    } else if (P.y  >= 0) {
        qDebug() << "Point is above the canvas.";
        return "above";
    } else if (P.y   <= canvasHeight) {
        qDebug() << "Point is below the canvas.";
        return "bottom";
        /*} else if (Lordered.back().x  >= 0 && Lordered.back().x <= canvasWidth && Lordered.back().y >= 0 && Lordered.back().y <= canvasHeight) {
                        qDebug() << "Point is inside the canvas.";*/
    } else {

        qDebug() << "Unknown point position.";
        return "none";
    }

}

Vector2D Vector2D::extendLineToCanvas(const Vector2D& M, const Vector2D& P, float canvasWidth, float canvasHeight) {
    // Direction de la droite MP
    Vector2D MP = P - M;

    // Calcul du paramètre t pour trouver l'intersection avec les bords du canvas

    // Si la direction est nulle, retourner le point M
    if (MP.x == 0 && MP.y == 0) {
        return M;
    }

    // Paramètres pour les intersections avec x = 0, x = canvasWidth, y = 0, y = canvasHeight
    float tX0 = -M.x / MP.x;
    float tX1 = (canvasWidth - M.x) / MP.x;
    float tY0 = -M.y / MP.y;
    float tY1 = (canvasHeight - M.y) / MP.y;

    // Trouver les points d'intersection en fonction des t
    Vector2D pX0 = M + MP * tX0;
    Vector2D pX1 = M + MP * tX1;
    Vector2D pY0 = M + MP * tY0;
    Vector2D pY1 = M + MP * tY1;

    // Vérifier quels points sont valides (dans les limites du canvas)
    Vector2D intersection;
    if (tX0 >= 0 && pX0.y >= 0 && pX0.y <= canvasHeight) {
        intersection = pX0; // Intersection avec x = 0
    }
    else if (tX1 >= 0 && pX1.y >= 0 && pX1.y <= canvasHeight) {
        intersection = pX1; // Intersection avec x = canvasWidth
    }
    else if (tY0 >= 0 && pY0.x >= 0 && pY0.x <= canvasWidth) {
        intersection = pY0; // Intersection avec y = 0
    }
    else if (tY1 >= 0 && pY1.x >= 0 && pY1.x <= canvasWidth) {
        intersection = pY1; // Intersection avec y = canvasHeight
    }

    return intersection;
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


