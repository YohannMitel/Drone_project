#include <QPainter>
#include "canvas.h"
#include <QMouseEvent>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QVector>
#include <mypolygon.h>
#include "vector2d.h"

Canvas::Canvas(QWidget *parent)
    : QWidget{parent} {
    droneImg.load("../../media/drone.png");
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
    for( auto d: mapDrones){
        delete d;
    }
    mapDrones.clear();
    triangles.clear();
    cities->clear();
}


void Canvas::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QBrush whiteBrush(Qt::SolidPattern);
    QPen penCol(Qt::DashDotDotLine);
    penCol.setColor(Qt::lightGray);
    penCol.setWidth(3);
    whiteBrush.setColor(Qt::white);
    painter.fillRect(0,0,width(),height(),whiteBrush);


    //painter.fillRect(0,0,width(),height(),whiteBrush);

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


    if (mapDrones.size() > 0 ) {
        Vector2D p;
        QRect rect(-droneIconSize/2,-droneIconSize/2,droneIconSize,droneIconSize);
        QRect rectCol(-droneCollisionDistance/2,-droneCollisionDistance/2,droneCollisionDistance,droneCollisionDistance);

        for (auto &drone:mapDrones) {
            painter.save();
            // place and orient the drone
            painter.translate(drone->getPosition().x,drone->getPosition().y);
            painter.rotate(drone->getAzimut());
            painter.drawImage(rect,droneImg);
            // light leds if flying
            if (drone->getStatus()!=Drone::landed) {
                painter.setPen(Qt::NoPen);
                painter.setBrush(Qt::red);
                painter.drawEllipse((-185.0/511.0)*droneIconSize,(-185.0/511.0)*droneIconSize,(65.0/511.0)*droneIconSize,(65.0/511.0)*droneIconSize);
                painter.drawEllipse((115.0/511.0)*droneIconSize,(-185.0/511.0)*droneIconSize,(65.0/511.0)*droneIconSize,(65.0/511.0)*droneIconSize);
                painter.setBrush(Qt::green);
                painter.drawEllipse((-185.0/511.0)*droneIconSize,(115.0/511.0)*droneIconSize,(70.0/511.0)*droneIconSize,(70.0/511.0)*droneIconSize);
                painter.drawEllipse((115.0/511.0)*droneIconSize,(115.0/511.0)*droneIconSize,(70.0/511.0)*droneIconSize,(70.0/511.0)*droneIconSize);
            }
            // draw collision detector
            if (drone->hasCollision()) {
                painter.setPen(penCol);
                painter.setBrush(Qt::NoBrush);
                painter.drawEllipse(rectCol);
            }
            painter.restore();
        }
    }

}

void Canvas::mousePressEvent(QMouseEvent *event) {
    // search for a drone that is landed
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


    auto it = mapDrones.begin();
    while (it!=mapDrones.end() && (*it)->getStatus()!=Drone::landed) {
        it++;
    }
    // if found, ask for a motion to the mouse position
    if (it!=mapDrones.end()) {
        (*it)->setGoalPosition(Vector2D(mouseX,mouseY));
        (*it)->start();
    }



    repaint();
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


void Canvas::loadMesh(const QString &title) {
    QFile file(title);

    if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        qDebug() << "ça sent l'roussi";
        clear();
        QString JSON=file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(JSON.toUtf8());
        QJsonArray JSONvertices = doc["servers"].toArray();
        QJsonArray JSONdrone =  doc["drones"].toArray();


        qDebug() << "Vertices:" << JSONvertices.size();
        qDebug() << "Drones:" << JSONdrone.size();



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


        /* preset initial positions of the drones */

        int n= 0;
        for (auto &&v:JSONdrone) {
            QJsonObject vector=v.toObject();


            QString name= vector["name"].toString();
            auto strPosition = vector["position"].toString().split(',');
            Vector2D pt(strPosition[0].toFloat(),strPosition[1].toFloat());
            Drone *drone = new Drone(name);
            drone->setInitialPosition(pt);
            mapDrones.push_back(drone);

        }


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
        return Vector2D(0, canvasHeight);
    }else if(result == ("right above" ) || result == "above right"){
        return Vector2D(canvasWidth, canvasHeight);
    }else if(result == ("right bottom" ) || result == "bottom right"){
        return Vector2D(canvasWidth, 0);
    }else if(result == ("left bottom" ) || result == "bottom left"){
        return Vector2D(0, 0);
    }else{
        return Vector2D(0,0);
    }
}

bool Canvas::isOutsideCanvas(const Vector2D &point) const{
    return point.x  < 0 ||  point.x > (width()-10)/scale+origin.x() ||
           point.y < 0 ||  point.y > (height()+10)/scale+origin.y();
}

Vector2D Canvas::calculateIntersection(const Vector2D &p1, const Vector2D &p2, float minX, float minY, float maxX, float maxY) {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;

    // Intersection points
    QVector<Vector2D> intersections;

    // Check vertical boundaries (minX and maxX)
    if (dx != 0) {
        float t1 = (minX - p1.x) / dx; // Left boundary
        if (t1 >= 0 && t1 <= 1) {
            intersections.append(Vector2D(minX, p1.y + t1 * dy));
        }
        float t2 = (maxX - p1.x) / dx; // Right boundary
        if (t2 >= 0 && t2 <= 1) {
            intersections.append(Vector2D(maxX, p1.y + t2 * dy));
        }
    }

    // Check horizontal boundaries (minY and maxY)
    if (dy != 0) {
        float t3 = (minY - p1.y) / dy; // Bottom boundary
        if (t3 >= 0 && t3 <= 1) {
            intersections.append(Vector2D(p1.x + t3 * dx, minY));
        }
        float t4 = (maxY - p1.y) / dy; // Top boundary
        if (t4 >= 0 && t4 <= 1) {
            intersections.append(Vector2D(p1.x + t4 * dx, maxY));
        }
    }

    // Choose the first valid intersection (there should only be one per edge)
    if (!intersections.isEmpty()) {
        return intersections.first();
    }

    // If no intersection found (should not happen), return the original point
    return p1;
}


void Canvas::finalizePolygon(City &city, const QVector<Vector2D> &Lordered, bool isClosed) {

    QVector<Vector2D> clippedVertices; // Store the clipped vertices
    const float canvasMinX = 0;
    const float canvasMinY = 0;
    const float canvasMaxX = (width() - 10) / scale + origin.x();
    const float canvasMaxY = (height() + 10) / scale + origin.y();

    auto clipEdge = [&](const Vector2D &p1, const Vector2D &p2, QVector<Vector2D> &output) {
        // Check if points are inside the canvas
        bool insideP1;// = isInsideCanvas(p1);
        bool insideP2;// = isInsideCanvas(p2);

        if(isOutsideCanvas(p1) == true){
            insideP1 = false;
        }else{
            insideP1 = true;
        }

        if(isOutsideCanvas(p2) == true){
            insideP2 = false;
        }else{
            insideP2 = true;
        }

        if (insideP1 && insideP2) {
            // Both points inside: add p2
            output.append(p2);
        } else if (insideP1 && !insideP2) {
            // Going out of bounds: add intersection point
            Vector2D intersection = calculateIntersection(p1, p2, canvasMinX, canvasMinY, canvasMaxX, canvasMaxY);
            output.append(intersection);
        } else if (!insideP1 && insideP2) {
            // Coming back in bounds: add intersection and p2
            Vector2D intersection = calculateIntersection(p1, p2, canvasMinX, canvasMinY, canvasMaxX, canvasMaxY);
            output.append(intersection);
            output.append(p2);
        }
        // If both points are outside, no points are added.
    };

    // Iterate over the polygon edges and clip each edge
    for (int i = 0; i < Lordered.size(); ++i) {
        const Vector2D &current = Lordered[i];
        const Vector2D &next = Lordered[(i + 1) % Lordered.size()]; // Next vertex (wrapping around for closed polygons)
        clipEdge(current, next, clippedVertices);
    }

    MyPolygon *poly = new MyPolygon(clippedVertices.size());
    // Add the clipped vertices to the polygon
    for (const Vector2D &vertex : clippedVertices) {

        poly->addVertex(vertex);
    }

    // Set the final polygon to the city
    city.setMap(poly);
    update();

    qDebug() << "Nombre de points trouvés " << city.getName() << " : " << clippedVertices.size();
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

    int computedLimitPointNb = 0 ;

    // Récupère tous les triangles autour du point
    for(auto &tri:triangles){
        if(tri->contains(P)){

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
                qDebug() << "ICI 1";
                Lordered.push_back((*it)->getCircleCenter());
            }else{

                if(!isOutsideCanvas((*it)->getCircleCenter()) && !isOutsideCanvas(T->getCircleCenter()) ) {
                    qDebug() << "ICI 2";

                    qDebug() << (*it)->getCircleCenter();
                    if(!Lordered.contains((*it)->getCircleCenter())) Lordered.push_front((*it)->getCircleCenter());


                    const Vector2D inter = Vector2D::getCanvasIntersectionLimit(P, *(*it)->getEdgeTo(P), (*it)->getCircleCenter(), canvasWidth, canvasHeight);
                    qDebug() << "ICI 3";
                    if(computedLimitPointNb < 2){
                        qDebug() << "computedLimitPointNb";
                        Lordered.push_front(inter);

                        computedLimitPointNb++;
                    }


                }else{
                    qDebug() << "ICI 4";
                    Lordered.push_front((*it)->getCircleCenter());
                }


            }

            T = *it;
            L.removeOne(T);

            /* La liste des triangles a parcourir est vide, le triangle est ouvert alors
                ont cherche le point qui se trouve à la limite du canvas
            */

            if (L.isEmpty() && !isClosed) {
                edge = T->getEdgeTo(P);
                const Vector2D circumCircle = T->getCircleCenter();
                if (isOutsideCanvas(circumCircle)) {

                    const QString side1 = Vector2D::whichSide(Lordered.back(), canvasWidth, canvasHeight);
                    const QString side2 = Vector2D::whichSide(circumCircle, canvasWidth, canvasHeight);
                    if (side1 != side2) {
                        qDebug() << "ICI 5";
                        Lordered.push_back(getBorderPointSide(side1, side2, canvasWidth, canvasHeight));
                    }
                    qDebug() << "ICI 6";
                    qDebug() << "Val " << circumCircle;
                    //Lordered.push_back(circumCircle);
                } else {

                    const Vector2D inter1 = Vector2D::extendLineToCanvas(*(Lordered.end() - 2), Lordered.back(), canvasWidth, canvasHeight);
                    const Vector2D inter2 = Vector2D::getCanvasIntersectionLimit(edge, P, circumCircle, canvasWidth, canvasHeight);

                    const QString side1 = Vector2D::whichSide(inter1, canvasWidth, canvasHeight);
                    const QString side2 = Vector2D::whichSide(inter2, canvasWidth, canvasHeight);
                    if (side1 != side2) {
                        qDebug() << "ICI 7";
                        Lordered.push_back(getBorderPointSide(side1, side2, canvasWidth, canvasHeight));
                        qDebug() << getBorderPointSide(side1, side2, canvasWidth, canvasHeight);
                    }

                    if(computedLimitPointNb < 2){
                        qDebug() << "ICI 8";
                        qDebug() << "computedLimitPointNb";
                        Lordered.push_back(inter2);
                        computedLimitPointNb++;
                    }

                }
            }
            /* Aucun voisin n'a été trouvé le triangles es tdonc ouvert  */
        } else {
            isClosed = false;

            const Vector2D circumCircle = T->getCircleCenter();
            if (isOutsideCanvas(circumCircle)) {
                qDebug() << "ICI 9";
                if(!Lordered.contains(circumCircle)) Lordered.push_back(circumCircle);

            } else {

                if(!Lordered.contains(circumCircle)) {
                    qDebug() << "ICI 10";
                    Lordered.push_front(circumCircle);
                }
                if(computedLimitPointNb < 2){
                    const Vector2D inter = Vector2D::getCanvasIntersectionLimit(edge, P, circumCircle, canvasWidth, canvasHeight);
                    qDebug() << "ICI 11";
                    qDebug() << "Val " << inter;
                    qDebug() << "computedLimitPointNb";
                    Lordered.push_back(inter);
                    computedLimitPointNb++;
                }

            }

            L.removeOne(T);

            if(L.isEmpty() ){

                edge = T->getEdgeTo(P);

                const Vector2D circumCircle = (T)->getCircleCenter();

                if(isOutsideCanvas(circumCircle)){
                    qDebug() << "ICI 12";
                    Lordered.push_back(circumCircle);

                }else if(computedLimitPointNb < 2){
                    const Vector2D inter = Vector2D::getCanvasIntersectionLimit(edge, P, circumCircle, canvasWidth, canvasHeight);
                    qDebug() << "ICI 13";
                    qDebug() << "computedLimitPointNb";

                    Lordered.push_back(inter);
                    computedLimitPointNb++;


                }

            }


        }
    }

    finalizePolygon(city, Lordered, isClosed);

}

void Canvas::processPoly(){
    flippAll();
    for(auto &c: cities->getTabCities()){
        processVoronoi(*c);
    }
    cities->connectionMatrix(cities->getTabCities());
    /*   qDebug() << "VORONOI DE : "  << cities->getTabCities()[1]->getName();
    processVoronoi(*cities->getTabCities()[1]);*/
}




