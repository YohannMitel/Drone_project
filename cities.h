#ifndef CITIES_H
#define CITIES_H
#include "city.h"
#include "mypolygon.h"

class Cities
{
private:
    QVector<City*> tabCities;
    MyPolygon *convexHull = nullptr;
public:
    Cities();
    ~Cities();

    qsizetype getSize() const;
    void clear();

    Vector2D* getPointByIndex(int index);
    void pushCity(QString name, Vector2D *position, QString color);

    /* allow us to order point to create a convex hull */
    QVector<QPair<QString,Vector2D>>  ascendingPolarAngle(Vector2D &porigin);


    /* SEE IF A POINT IS ON THE LEFT */
    bool isOnTheLeft(const Vector2D &P, const Vector2D &top_1, const Vector2D &top);

    void orderPolygonPoint(Vector2D &porigin);
    static bool sorting(City &City, QPair<QString,Vector2D>  Pair );
    QVector<City*> getTabCities();
    QVector<Vector2D*> getTabVertices();
    QVector<Triangle*> initTriangulation();

    void draw(QPainter &painter, bool transparency);

};

#endif // CITIES_H
