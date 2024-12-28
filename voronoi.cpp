#include <voronoi.h>


Voronoi::Voronoi(){

}

Voronoi::~Voronoi(){

}
void Voronoi::draw(QPainter &painter, bool transparency){

    for (auto &poly: tabPolygon) {

        poly->draw(painter, transparency);
    }
}

