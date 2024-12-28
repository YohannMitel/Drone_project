#include "vector2d.h"

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
