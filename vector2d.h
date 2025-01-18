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

    bool operator<(const Vector2D& other) const {
        if (x < other.x) {
            return true;
        } else if (x == other.x) {
            return y < other.y;
        }
        return false;
    }

    /* Fonction pour faire la projection orthogonale, nous permettant de trouver la perpendiculaire entre le point et le vecteur
        il nous sera retourné le point d'intersection sur le vecteur
    */
    static Vector2D projection(const Vector2D &A, const Vector2D &B, const Vector2D &M);

    // Fonction pour étendre la droite MP jusqu'à la limite du canvas
    static Vector2D extendLineToCanvas(const Vector2D& M, const Vector2D& P, float canvasWidth, float canvasHeight);

    // Permet de trouver sur quel bord du canvas se trouve le point
    static QString whichSide(const Vector2D &P, float canvasWidth, float canvasHeight);


    static bool polarComparison(QPair<QString,Vector2D> P1,QPair<QString,Vector2D> P2);

    // Calcul la projection et applique la fonction extendLine to Canvas
    static Vector2D getCanvasIntersectionLimit(const Vector2D &A, const Vector2D &B, const Vector2D &M, float canvasWidth, float canvasHeight) ;

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
