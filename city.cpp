#include "city.h"

City::City(QString _name, Vector2D *_position, QString _color ){
    name = _name;
    position = _position;
    color = _color;
};

City::~City(){
    delete position;
    delete map;
}

/* Getters */

QString City::getName() const{
    return name;
};

Vector2D* City::getPosition(){
    return position;
};

QString City::getColor(){
    return color;
};

MyPolygon* City::getMap(){
    return map;
}

/* Setters */

void City::setMap(MyPolygon *poly){
    map = poly;
}

void City::drawMap(QPainter &painter, bool transparency){
    if(map == nullptr) return;

    map->draw(painter, transparency, color);
}


