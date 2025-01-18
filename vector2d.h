/**
 * @brief Drone_demo project
 * @author B.Piranda
 * @date dec. 2024
 **/
#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>
#include <QDebug>

class Vector2D {
public:
    float x,y; ///< coordinates of the vector
    Vector2D(float p_x,float p_y):x(p_x),y(p_y) {};
    Vector2D() { x=y=0.0; }
    Vector2D(Vector2D *p):x(p->x),y(p->y) {};

    /**
     * @brief set new coordinates to the vector
     * @param p_x: x component
     * @param p_y: y component
     */
    void set(float p_x,float p_y) { x=p_x; y=p_y; }
    /**
     * @brief length: return the length (or the norm) of the vector
     * @return the length
     */
    double length() const {
        return sqrt(x*x+y*y);
    }
    /**
     * @brief normalize: change the current vector to a vector with the same direction but a norme equal to 1
     */
    void normalize() {
        float l=float(length());
        x/=l;
        y/=l;
    }
    /**
     * @brief orthoNormed: return a new vector, orthogonal and normed
     * @return the orthonormed vector
     */
    Vector2D orthoNormed() const {
        float l=float(length());
        return Vector2D(y/l,-x/l);
    }
    /**
     * @brief operator []: a way to get the component of the vector
     * @param i: equal to 0 or 1
     * @return x if i is equal to 0 and y else
     */
    float operator[](const int i) const {
        return (i==0)?x:y;
    };
    /**
     * @brief operator +=: multiply the components of the current vector with the components of the vector v
     * @param v: the multiplier
     */
    void operator+=(const Vector2D& v) {
        x+=v.x;
        y+=v.y;
    }


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
