#include "cities.h"

Cities::Cities() {

}
Cities::~Cities() {
    for(City* c: tabCities){
        delete c;
    }
}


qsizetype Cities::getSize() const {
    return tabCities.size();
};

void Cities::clear(){
    tabCities.clear();
}

void Cities::pushCity(QString name, Vector2D *position, QString color){
    tabCities.push_back(new City(name,position,color));
}

Vector2D* Cities::getPointByIndex(int index){
    return tabCities[index]->getPosition();
}

void Cities::draw(QPainter &painter, bool transparency){
    for(auto &c: tabCities){
        c->drawMap( painter,  transparency);
    }
}

QVector<City*> Cities::getTabCities(){
    return tabCities;
}

QVector<Vector2D*> Cities::getTabVertices(){
    QVector<Vector2D *> tabVertices;

    for(auto &v: tabCities){
        tabVertices.push_back(v->getPosition());
    }
    return tabVertices;
}



bool Cities::isOnTheLeft(const Vector2D &P, const Vector2D &top_1, const Vector2D &top) {

    Vector2D AB =  top_1 - top ,
        AP = top - P ;

    /*qDebug() << "AB" << AB;
    qDebug() << "AP" << AP;*/
    return (AB.x*AP.y-AB.y*AP.x)>=0;
}

bool Cities::sorting(City &City, QPair<QString,Vector2D>  Pair ){
    return City.getName() < Pair.first ;

}

QVector<QPair<QString,Vector2D>> Cities::ascendingPolarAngle(Vector2D &porigin) {


    QVector<QPair<QString,Vector2D>> pointsRelative;
    for (auto &v:tabCities) {
        auto pOrig = *(v->getPosition());
        qDebug() << "pOrig : " << pOrig.y << " porigin : " << porigin.y;
        pointsRelative.append(qMakePair(v->getName(),Vector2D(pOrig.x - porigin.x, pOrig.y - porigin.y) ));
    }
    // sorting point with angular criteria
    std::sort(pointsRelative.begin()+1, pointsRelative.end(),Vector2D::polarComparison);

    for(auto &p: pointsRelative){
        qDebug() << p;
    }

    return pointsRelative;

}



void  Cities::orderPolygonPoint(Vector2D &porigin){
    auto pointsRelativeList = ascendingPolarAngle(porigin);
    int N,Nmax;
    delete convexHull;

    QVector<QPair<QString,Vector2D>> CHstack;
    QPair<QString,Vector2D> top, top_1;

    CHstack.append(pointsRelativeList[0]);
    CHstack.append(pointsRelativeList[1]);
    CHstack.append(pointsRelativeList[2]);
    auto it=pointsRelativeList.begin()+3;

    while (it!=pointsRelativeList.end()) {
        top = CHstack.last(); // extract top and top_1
        CHstack.pop_back();
        // from the stack
        qDebug() << CHstack;
        top_1 = CHstack.last();
        CHstack.push_back(top);


        while (!isOnTheLeft((*it).second,top_1.second,top.second)) {

            CHstack.pop_back(); // update top and top_1
            top = CHstack.last();
            CHstack.pop_back();
            top_1 = CHstack.last();
            CHstack.push_back(top);

        }

        CHstack.push_back((*it));
        it++;
    }
    // get stack points to create current polygon
    //qDebug() <<"CHstack" << CHstack;


    N=CHstack.size();
    Nmax = N;

    convexHull = new MyPolygon(Nmax);

    while (!CHstack.empty()) {
        const Vector2D v = CHstack.last().second;

        convexHull->addVertex(v);
        CHstack.pop_back();
    }



}

QVector<Triangle*> Cities::initTriangulation(){

    QVector<Vector2D *> tabVert = getTabVertices() ;

    return  convexHull->earClipping(tabVert );;
}



