#include <voronoi.h>


Voronoi::Voronoi(){

}

Voronoi::~Voronoi(){

}
void Voronoi::draw(QPainter &painter){

    for (auto &poly: tabPolygon) {

        poly->draw(painter);
    }
}

