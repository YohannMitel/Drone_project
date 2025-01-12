#include "canvas.h"
#include <QMouseEvent>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QVector>
#include <mypolygon.h>
#include "vector2d.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);

}

Canvas::~Canvas() {
    clear();
    for (Triangle* tri : triangles) {
        delete tri;  // Libère la mémoire allouée pour chaque Triangle
    }
    delete cities;
}

void Canvas::clear() {
    triangles.clear();
    cities->clear();
}

void Canvas::addPoints(QString &name ,const QVector<Vector2D> &tab) {
    for (auto &pt:tab) {
        // duplicate the point to get a local permanent version
        cities->pushCity(name ,new Vector2D(pt), "");
    }
    reScale();
    update();
}

void Canvas::addTriangle( int id0, int id1, int id2) {

    triangles.push_back(new Triangle(cities->getPointByIndex(id0),cities->getPointByIndex(id1),cities->getPointByIndex(id2)));
}

void Canvas::addTriangle(int id0, int id1, int id2,const QColor &color) {
    triangles.push_back(new Triangle(cities->getPointByIndex(id0),cities->getPointByIndex(id1),cities->getPointByIndex(id2),color));
}

void Canvas::paintEvent(QPaintEvent *) {
    //qDebug() << "PAINT";
    QPainter painter(this);
    QBrush whiteBrush(Qt::SolidPattern);
    whiteBrush.setColor(Qt::white);
    painter.fillRect(0,0,width(),height(),whiteBrush);

    // draw axes
    QPointF points[7]={{0,-2},{80,-2},{80,-10},{100,0},{80,10},{80,2},{0,2}};
    painter.save();
    painter.translate(20,height()-20);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::red);
    painter.drawPolygon(points,7);
    painter.save();
    painter.setBrush(Qt::green);
    painter.rotate(-90);
    painter.drawPolygon(points,7);
    painter.restore();
    painter.restore();

    painter.save();
    painter.translate(10,height()-10);
    painter.scale(scale,-scale);
    painter.translate(-origin.x(),-origin.y());

    // draw triangles
    if (showTriangles) {
        //qDebug() << triangles;
        for (auto &tri:triangles) {
            tri->draw(painter);
        }

    }

    // draw circle
    if (showCircles) {
        for (auto &tri:triangles) {
            if (tri->isHighlighted()) tri->drawCircle(painter);
        }
    }
    // draw points
    if (showCenters) {
        painter.setPen(QPen(Qt::black,3));
        for (auto &tri:triangles) {
            auto pt = tri->getCircleCenter();
            painter.drawLine(pt.x-15,pt.y-15,pt.x+15,pt.y+15);
            painter.drawLine(pt.x-15,pt.y+15,pt.x+15,pt.y-15);
        }
    }    // Draw voronoi polygons

    cities->draw(painter, voronoiTransparency);

    painter.restore();

    // draw the text in basic coordinate system
    int s=width()/100;
    QFont font("Times",s,QFont::Normal);
    painter.setFont(font);
    painter.setPen(QPen(Qt::black));
    const QRect rect(-8*s,-3.5*s,10*s,2.5*s);

    for (auto &v:cities->getTabCities()) {
        Vector2D pts = v->getPosition() ;
        //TELEPORT

        painter.save();
        float x = (pts.x - origin.x())*scale+10+1.5*s;
        float y = -(pts.y - origin.y())*scale+height()-10+1.25*s;



        painter.translate(x,y);
        painter.fillRect(rect,QBrush(QColor(255,255,255,192)));
        painter.drawText(rect,Qt::AlignCenter|Qt::AlignVCenter,v->getName());
        painter.restore();
    }

}

QPair<Vector2D,Vector2D> Canvas::getBox() {
    auto vertices = cities->getTabCities();
    if (vertices.empty()) {Vector2D infLeft,supRight;
        return QPair<Vector2D,Vector2D>(Vector2D(0,0),Vector2D(200,200));
    }
    auto index= vertices.begin();
    auto pts = *(*index)->getPosition();
    auto indexEnd= vertices.end();
    //auto ptsEnd = (*indexEnd)->getPosition();
    Vector2D infLeft(pts.x,pts.y),supRight(pts.x,pts.y);
    while ( index != indexEnd ) {
        pts = (*index)->getPosition();
        //qDebug() <<  "PTS : " << pts << "  inf : " << infLeft;
        if (pts.x<infLeft.x) infLeft.x= pts.x;
        if (pts.y<infLeft.y) infLeft.y= pts.y;
        if (pts.x>supRight.x) supRight.x= pts.x;
        if (pts.y>supRight.y) supRight.y= pts.y;
        index++;
        //qDebug() << "FF";
    }

    return QPair<Vector2D,Vector2D>(infLeft,supRight);
}

void Canvas::resizeEvent(QResizeEvent *event) {
    reScale();
    update();
}

void Canvas::reScale() {
    //qDebug() << "RESCALE";
    int newWidth = width()-20;
    int newHeight = height()-20;

    auto box=getBox();
    // qDebug() << box;
    float dataWidth=box.second.x-box.first.x;
    float dataHeight=box.second.y-box.first.y;
    scale=qMin(float(newWidth)/float(dataWidth),float(newHeight)/float(dataHeight)  );
    origin.setX(box.first.x);
    origin.setY(box.first.y);
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    float mouseX=float(event->pos().x()-10)/scale+origin.x();
    float mouseY=-float(event->pos().y()-height()+10)/scale+origin.y();
    emit updateSB(QString("Mouse position= (") + QString::number(mouseX, 'f', 1) + "," + QString::number(mouseY, 'f', 1) + ")");

    for (auto &tri:triangles) {
        tri->setHighlighted(tri->isInside(mouseX,mouseY));
    }
    update();
}

void Canvas::mousePressEvent(QMouseEvent * event){

    float mouseX=float(event->pos().x()-10)/scale+origin.x();
    float mouseY=-float(event->pos().y()-height()+10)/scale+origin.y();

    for (auto &tri:triangles) {

        if(tri->isInside(mouseX,mouseY) && tri->isFlippable()){

            tri->flippIt(triangles);
            checkDelaunay();
            qDebug() << "FlippIt ended";
            break;
        }
    }


}

void Canvas::loadMesh(const QString &title) {
    QFile file(title);

    if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
            qDebug() << "ça sent l'roussi";
        clear();
        QString JSON=file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(JSON.toUtf8());
        QJsonArray JSONvertices = doc["servers"].toArray();


        qDebug() << "Vertices:" << JSONvertices.size();



        for (auto &&v:JSONvertices) {

            QJsonObject vector=v.toObject();
            qDebug() << vector["position"].toString() << "," << vector["name"].toString();
            auto strPosition = vector["position"].toString().split(',');
            Vector2D pt(strPosition[0].toFloat(),strPosition[1].toFloat());
            auto servName = vector["name"].toString();
            auto color = vector["color"].toString();
            //if(servName == "Freljord")  origin = pt;
            cities->pushCity(servName, new Vector2D(pt.x, pt.y), color);

        }

        QVector<Vector2D*> vertices = cities->getTabVertices();
        if (vertices.isEmpty()) {
            throw std::runtime_error("Le QVector est vide, impossible de trouver un point.");
        }

        Vector2D   origin = vertices[0]; // Initialiser avec le premier point
        for (const auto &p : vertices) {
            if (p->y < origin.y) { // Comparer les coordonnées y
                origin = p;
            }
        }

        //qDebug() << origin;


        triangles.clear();
        cities->orderPolygonPoint(origin);
        triangles = cities->initTriangulation();


    }
    reScale();
    update();
}

QVector<const Vector2D*> Canvas::findOppositePointOfTrianglesWithEdgeCommon(const Triangle &tri){
    QVector<const Vector2D*> res;
    for(auto t:triangles){
        if(  tri.hasEdge(t->getVertexPtr(1),t->getVertexPtr(0)) ){
            res.push_back(t->getVertexPtr(2));
        }
        else if(tri.hasEdge(t->getVertexPtr(2),t->getVertexPtr(1))){

            res.push_back(t->getVertexPtr(0));
        }
        else if(tri.hasEdge(t->getVertexPtr(0),t->getVertexPtr(2)) ){
            res.push_back(t->getVertexPtr(1));
        };

    }
    return res;
}

bool Canvas::checkDelaunay(){
    qDebug()<< "Delaunay process";

    bool areAllDelaunay = true;
    auto vertices = cities->getTabVertices();
    for(auto &tri:triangles){
        bool res = tri->checkDelaunay(vertices);
        //qDebug() << res;

        // Check if the triangle is flippable
        if(!res){
            auto L=findOppositePointOfTrianglesWithEdgeCommon(*tri);
            //qDebug() << "L.size:" << L.size();

            auto it=L.begin();

            while (it != L.end() && tri->circleContains(*it)) {
                // qDebug() << "pt:" << (*it)->x << "," << (*it)->y;
                ++it; // Avancer l'itérateur
            }

            // Vérifier si l'itérateur est valide et que l'élément courant ne satisfait pas circleContains
            if (it != L.end()) {
                tri->setOpposite(const_cast<Vector2D*>(*it));
            }


            //qDebug() << "L.size:" << L.size() << " condition : " << (it!=L.end());

            tri->setDelaunay(false,it!=L.end());
        }
        /////////////////////////////////////////////////////////////////////////

        areAllDelaunay = areAllDelaunay && res;

    }
    update();
    return areAllDelaunay;
}

void Canvas::flippAll(){
    while(!checkDelaunay()){
        auto it  = triangles.begin();
        while( it !=triangles.end() && !(*it)->isFlippable()){
            it++;
        }
        if(it!=triangles.end()){
            (*it)->flippIt(triangles);

        }else{
            qDebug() << "issue";
        }
    }
}

Vector2D getBorderPointSide(QString sideP1, QString sideP2, const float &canvasWidth, const float &canvasHeight){
    const QString result = sideP1 +  " " +sideP2;

    if(result == ("left above" ) || result == "above left"){
        return new Vector2D(0, canvasHeight);
    }else if(result == ("right above" ) || result == "above right"){
        return new Vector2D(canvasWidth, canvasHeight);
    }else if(result == ("right bottom" ) || result == "bottom right"){
        return new Vector2D(canvasWidth, 0);
    }else if(result == ("left bottom" ) || result == "bottom left"){
        return new Vector2D(0, 0);
    }else{
        return new Vector2D(0,0);
    }
}

bool Canvas::isOutsideCanvas(const Vector2D &point) const{
    return point.x  < 0 ||  point.x > (width()-10)/scale+origin.x() ||
           point.y < 0 ||  point.y > (height()+10)/scale+origin.y();
}


void Canvas::finalizePolygon(City &city, const QVector<Vector2D> &Lordered, bool isClosed) {
    MyPolygon *poly = new MyPolygon(Lordered.size());
    for (const Vector2D &vertex : Lordered) {
        poly->addVertex(vertex);
    }

    city.setMap(poly);
    update();

    qDebug() << "Nombre de points trouvés : " << Lordered.size();
    qDebug() << "La cellule de Voronoi est " << (isClosed ? "FERMÉE" : "OUVERTE");
}

void Canvas::processVoronoi(City &city){
    Vector2D P = city.getPosition();
    qDebug() << "Processing polygons : " << city.getName();
    bool isClosed = true; // Initialement, on suppose que la cellule est fermée

    Vector2D *edge = nullptr;
    QVector <Triangle *> L;

    const float canvasWidth = (width()-10)/scale+origin.x();
    const float canvasHeight = (height()+10)/scale+origin.y();

    // Récupère tous les triangles autour du point
    for(auto &tri:triangles){
        if(tri->contains(P)){
            /*qDebug() << "Points triangle : ";
            qDebug() << *(tri->getVertexPtr(0));
            qDebug() << *(tri->getVertexPtr(1));
            qDebug() << *(tri->getVertexPtr(2));*/
            L.push_back(tri);
        }
    }

    // Liste des circumcircle autour du triangle trié
    QVector<Vector2D> Lordered;


    // Si aucun triangle trouvé, on arrête
    assert(!L.isEmpty());

    // to do later : search the left Triangle

    auto T = L.back();
    while(!L.isEmpty() ){

        // search edge of T from P
        /*qDebug() << "Triangle de départ";
        qDebug() << *(T->getVertexPtr(0));
        qDebug() << *(T->getVertexPtr(1));
        qDebug() << *(T->getVertexPtr(2));*/

        /* Si l'on sait que le triangle est fermé alors on cherche le prochain triangle a droite
            sinon ont fait demi-tour pour chercher les triangles a gauche

        */
        edge = isClosed ? T->getEdgeFrom(P) : T->getEdgeTo(P);
        qDebug() << "Coordonnée recherchée" << *edge;


        // search triangle Tright wich is on the right of P

        auto it=L.begin();


        if(isClosed){
            qDebug () << "Recherche vers la droite";
            while(it!=L.end() && !(*(*it)->getEdgeTo(P) == *edge)){

                it++;

            }
        }else{
            qDebug () << "Recherche vers la gauche";
            while(it!=L.end() && !(*(*it)->getEdgeFrom(P) == *edge)){

                it++;

            }
        }


        // Un triangle voisin a été trouvé !
        if(it!=L.end()){
            qDebug() << "Voisin trouvé !!";
            if(isClosed){
                Lordered.push_back((*it)->getCircleCenter());
            }else{

                qDebug() << "JE SUIS ICI : " << isOutsideCanvas((*it)->getCircleCenter());;

                Lordered.push_front((*it)->getCircleCenter());


                if(!isOutsideCanvas((*it)->getCircleCenter())) {
                    const Vector2D proj = Vector2D::projection(P, *(*it)->getEdgeTo(P), (*it)->getCircleCenter());
                    const Vector2D inter = Vector2D::extendLineToCanvas((*it)->getCircleCenter(), proj, canvasWidth, canvasHeight);
                    Lordered.push_front(inter);

                    qDebug() << "EDGE : " << *(*it)->getEdgeTo(P);
                    qDebug() << "P : " << P;
                    qDebug()  << "PROJ : " << proj;
                    qDebug() << "circumCircle :  " << (*it)->getCircleCenter();
                    qDebug() << "INTERSECTION POINT : " << inter;
                }


            }

            T = *it;
            L.removeOne(T);

            /* La liste des triangles a parcourir est vide et le triangle est ouvert alors
                ont cherche le point qui se trouve à la limite du canvas
            */
            if (L.isEmpty() && !isClosed) {
                edge = T->getEdgeTo(P);
                const Vector2D circumCircle = T->getCircleCenter();
                if (isOutsideCanvas(circumCircle)) {

                    const QString side1 = Vector2D::whichSide(Lordered.back(), canvasWidth, canvasHeight);
                    const QString side2 = Vector2D::whichSide(circumCircle, canvasWidth, canvasHeight);
                    if (side1 != side2) {
                        Lordered.push_back(getBorderPointSide(side1, side2, canvasWidth, canvasHeight));
                    }
                    Lordered.push_back(circumCircle);
                } else {
                    qDebug() << "JE SUIS LA ";
                    const Vector2D proj = Vector2D::projection(edge, P, circumCircle);
                    const Vector2D inter1 = Vector2D::extendLineToCanvas(*(Lordered.end() - 2), Lordered.back(), canvasWidth, canvasHeight);
                    const Vector2D inter2 = Vector2D::extendLineToCanvas(circumCircle, proj, canvasWidth, canvasHeight);

                    qDebug( ) << "INTER 1 : " << inter1;
                    qDebug( ) << "INTER 2 : " << inter2;
                    const QString side1 = Vector2D::whichSide(inter1, canvasWidth, canvasHeight);
                    const QString side2 = Vector2D::whichSide(inter2, canvasWidth, canvasHeight);
                    if (side1 != side2) {
                        Lordered.push_back(getBorderPointSide(side1, side2, canvasWidth, canvasHeight));
                        qDebug() << getBorderPointSide(side1, side2, canvasWidth, canvasHeight);
                    }
                    Lordered.push_back(inter2);
                }
            }
        } else {
            isClosed = false;
            const Vector2D circumCircle = T->getCircleCenter();
            if (isOutsideCanvas(circumCircle)) {

                Lordered.push_back(circumCircle);
            } else {

                Lordered.push_front(circumCircle);
                const Vector2D proj = Vector2D::projection(edge, P, circumCircle);
                const Vector2D inter = Vector2D::extendLineToCanvas(circumCircle, proj, canvasWidth, canvasHeight);
                Lordered.push_back(inter);
            }
            L.removeOne(T);

            if(L.isEmpty() ){

                edge = T->getEdgeTo(P);

                const Vector2D circumCircle = (T)->getCircleCenter();

                if(isOutsideCanvas(circumCircle)){

                    Lordered.push_back(circumCircle);

                }else{
                    const Vector2D proj =  Vector2D::projection(edge,P, circumCircle);
                    const Vector2D inter =   Vector2D::extendLineToCanvas(circumCircle, proj, (width()-10)/scale+origin.x(), (height()+10)/scale+origin.y());

                    Lordered.push_back(inter);
                }

            }


        }
    }
    finalizePolygon(city, Lordered, isClosed);

}

void Canvas::processPoly(){
for(auto &c: cities->getTabCities()){
        processVoronoi(*c);
    }
  /* qDebug() << "VORONOI DE : "  << cities->getTabCities()[0]->getName();
    processVoronoi(*cities->getTabCities()[0]);*/
}
