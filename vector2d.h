#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>
#include <QDebug>

class Vector2D {
public:
    float x,y;
    Vector2D(float p_x,float p_y):x(p_x),y(p_y) {};
    Vector2D() { x=y=0.0; }
    Vector2D(Vector2D *p):x(p->x),y(p->y) {};

    void set(double p_x,double p_y) { x=p_x; y=p_y; }
    double length() const {
        return sqrt(x*x+y*y);
    }
    Vector2D orthoNormed() const {
        float l=length();
        return Vector2D(y/l,-x/l);
    }

    float operator[](const int i) const {
        return (i==0)?x:y;
    };

    friend double operator *(const Vector2D&,const Vector2D&);
    friend const Vector2D operator *(double,const Vector2D&);
    friend const Vector2D operator +(const Vector2D&,const Vector2D&);
    friend const Vector2D operator -(const Vector2D&,const Vector2D&);
    friend const Vector2D operator -(const Vector2D&);
    friend double operator ^(const Vector2D&,const Vector2D&);
    friend bool operator ==(const Vector2D&,const Vector2D&);
    friend bool operator !=(const Vector2D&,const Vector2D&);
    friend QDebug operator<<(QDebug dbg, const Vector2D& v) {
        dbg.nospace() << "Vector2D(" << v.x << ", " << v.y << ")";
        return dbg.space();
    }
    // Fonction pour déterminer si un point est dans un rectangle (canvas)
    bool isInsideCanvas(float width, float height) const {
        return x >= 0 && x <= width && y >= 0 && y <= height;
    }

    // A NETTOYER
    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    float dot(const Vector2D &op) {
        return x * op.x + y * op.y;
    }

    static Vector2D projection(const Vector2D &A, const Vector2D &B, const Vector2D &M) {
        Vector2D AB = B - A;   // Vector AB
        Vector2D AM = M - A;   // Vector AM

        // Projection factor t
        float t = AM.dot(AB) / (AB.length() * AB.length());

        // Clamp t to the range [0, 1]
        t = std::max(0.0f, std::min(1.0f, t));

        // Point H = A + t * AB
        return A + AB * t;
    }
    // Fonction pour étendre la droite MP jusqu'à la limite du canvas
    static Vector2D extendLineToCanvas(const Vector2D& M, const Vector2D& P, float canvasWidth, float canvasHeight) {
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





//////////////////////////////////


};

const Vector2D operator *(double,const Vector2D&);
double operator *(const Vector2D&,const Vector2D&);
const Vector2D operator +(const Vector2D&,const Vector2D&);
const Vector2D operator -(const Vector2D&,const Vector2D&);
const Vector2D operator -(const Vector2D&);
double operator ^(const Vector2D&,const Vector2D&);
bool operator==(const Vector2D&,const Vector2D&);
bool operator !=(const Vector2D&,const Vector2D&);
static bool edgesMatch(const Vector2D* p1, const Vector2D* p2, const Vector2D* p3, const Vector2D* p4);



#endif // VECTOR2D_H
