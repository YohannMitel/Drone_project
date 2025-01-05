#ifndef CITY_H
#define CITY_H
#include <QString>
#include "vector2d.h"
#include "mypolygon.h"

class City
{
private:
    QString name;
    Vector2D *position = nullptr;
    QString color;
    MyPolygon *map = nullptr;

public:
    City(QString _name, Vector2D *_position, QString _color );
    ~City();

    QString getName() const;
    Vector2D* getPosition();
    QString getColor();
    MyPolygon* getMap();


    /*void setName(QString &name);
    void setPosition(Vector2D *position);
    void setColor(QString &color);*/
    void setMap(MyPolygon *poly);

    void drawMap(QPainter &painter, bool transparency);

};

#endif // CITY_H
