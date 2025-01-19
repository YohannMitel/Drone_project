#ifndef CITIES_H
#define CITIES_H
#include "city.h"
#include "mypolygon.h"

class Cities
{
private:
    QVector<City*> tabCities;
    MyPolygon *convexHull = nullptr;
    QVector<QVector<int>> adjacencyMatrix;
    float heightLimit;
    float widthLimit;
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
    static bool sortingByPointsRelative(const City* city1, const City* city2,
                                        const QVector<QPair<QString, Vector2D>>& pointsRelative);
    QVector<City*> getTabCities();
    QVector<Vector2D*> getTabVertices();
    QVector<Triangle*> initTriangulation();

    void connectionMatrix(const QVector<City*>& cities);
    bool areNeighbors(Vector2D* vertices1, int numVertices1, Vector2D* vertices2, int numVertices2);
    bool doIntersect(Vector2D p1, Vector2D p2, Vector2D q1, Vector2D q2);
    int orientation(Vector2D p, Vector2D q, Vector2D r);
    int getCityByName(QString &name);

    void draw(QPainter &painter, bool transparency);
    void testPathFinding(int start, int end);

    Vector2D nextDestCityId(int start, int end);

    void setLimit(int &width, int &height){
        widthLimit = width;
        heightLimit = height;
    }

    bool isOutsideCities(const Vector2D &point) const{
        return point.x  < 0 ||  point.x > (widthLimit-10) ||
               point.y < 0 ||  point.y > (heightLimit+10);
    }

};

#endif // CITIES_H
