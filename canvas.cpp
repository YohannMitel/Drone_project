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
    delete voronoiObj;
}

void Canvas::clear() {
    triangles.clear();
    vertices.clear();
}

void Canvas::addPoints(const QVector<Vector2D> &tab) {
    for (auto &pt:tab) {
        // duplicate the point to get a local permanent version
        vertices.push_back(Vector2D(pt));
    }
    reScale();
    update();
}

void Canvas::addTriangle(int id0, int id1, int id2) {
    triangles.push_back(new Triangle(&vertices[id0],&vertices[id1],&vertices[id2]));
}

void Canvas::addTriangle(int id0, int id1, int id2,const QColor &color) {
    triangles.push_back(new Triangle(&vertices[id0],&vertices[id1],&vertices[id2],color));
}

void Canvas::paintEvent(QPaintEvent *) {
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

    voronoiObj->draw(painter, voronoiTransparency);
    painter.restore();

    // draw the text in basic coordinate system
    int s=width()/100;
    QFont font("Times",s,QFont::Normal);
    painter.setFont(font);
    painter.setPen(QPen(Qt::black));
    const QRect rect(-3*s,-2.5*s,3*s,2.5*s);
    int i=0;
    for (auto &v:vertices) {
        painter.save();
        float x = (v.x-origin.x())*scale+10+1.5*s;
        float y = -(v.y-origin.y())*scale+height()-10+1.25*s;
        painter.translate(x,y);
        painter.fillRect(rect,QBrush(QColor(255,255,255,192)));
        painter.drawText(rect,Qt::AlignCenter|Qt::AlignVCenter,QString::number(i++));
        painter.restore();
    }

}

QPair<Vector2D,Vector2D> Canvas::getBox() {
    if (vertices.empty()) {Vector2D infLeft,supRight;
        return QPair<Vector2D,Vector2D>(Vector2D(0,0),Vector2D(200,200));
    }
    auto pts=vertices.begin();
    Vector2D infLeft(pts->x,pts->y),supRight(pts->x,pts->y);
    while (pts!=vertices.end()) {
        if (pts->x<infLeft.x) infLeft.x=pts->x;
        if (pts->y<infLeft.y) infLeft.y=pts->y;
        if (pts->x>supRight.x) supRight.x=pts->x;
        if (pts->y>supRight.y) supRight.y=pts->y;
        pts++;
    }
    return QPair<Vector2D,Vector2D>(infLeft,supRight);
}

void Canvas::resizeEvent(QResizeEvent *event) {
    reScale();
    update();
}

void Canvas::reScale() {
    int newWidth = width()-20;
    int newHeight = height()-20;
    auto box=getBox();
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
        clear();
        QString JSON=file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(JSON.toUtf8());
        QJsonArray JSONvertices = doc["vertices"].toArray();
        vertices.resize(JSONvertices.size());
        qDebug() << "Vertices:" << JSONvertices.size();
        for (auto &&v:JSONvertices) {
            QJsonObject vector=v.toObject();
            qDebug() << vector["position"].toString() << "," << vector["id"].toInt();
            auto strPosition = vector["position"].toString().split(',');
            Vector2D pt(strPosition[0].toFloat(),strPosition[1].toFloat());
            auto intId = vector["id"].toInt();
            vertices[intId]=pt;
        }

        QJsonArray JSONtriangles = doc["triangles"].toArray();
        qDebug() << "Triangles:" << JSONtriangles.size();
        for (auto &&v:JSONtriangles) {
            QJsonObject vector=v.toObject();
            qDebug() << vector["tri"].toString() << "," << vector["color"];
            auto tri = vector["tri"].toString().split(',');
            auto color = vector["color"].toString();
            if (tri.size()==3) {
                addTriangle(tri[0].toInt(),tri[1].toInt(),tri[2].toInt(),QColor(color));
            }
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
    bool isClosed = true;
    bool areAllDelaunay = true;
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

void Canvas::processVoronoi(Vector2D &P){
    qDebug() << "Processing polygons";
    bool isClosed = true; // Initialement, on suppose que la cellule est fermée

    Vector2D *edge = nullptr;
    QVector <Triangle *> L;

    //poly = new MyPolygon()
    for(auto &tri:triangles){
        if(tri->contains(P)){
            qDebug() << "Points triangle : ";
            qDebug() << *(tri->getVertexPtr(0));
            qDebug() << *(tri->getVertexPtr(1));
            qDebug() << *(tri->getVertexPtr(2));
            L.push_back(tri);
        }
    }
    // POUR CETTE PARTIE DEMANDER A COPAIN POUR BIEN COMPRENDRE
    // is L closed ?
    QVector<Vector2D> Lordered;
    // to do later : search the left Triangle
    assert(!L.isEmpty());


    auto T = L.back();
    while(!L.isEmpty() && isClosed){

        // search edge of T from P
        /*qDebug() << "Triangle de départ";
        qDebug() << *(T->getVertexPtr(0));
        qDebug() << *(T->getVertexPtr(1));
        qDebug() << *(T->getVertexPtr(2));*/
        edge = T->getEdgeFrom(P);
        qDebug() << "Coordonnée recherché" << *edge;

        // search triangle Tright wich is on the right of P

        auto it=L.begin();


        while(it!=L.end() && !(*(*it)->getEdgeTo(P) == *edge)){

            it++;

        }
        qDebug() << L.size();
        if(it!=L.end()){
            Lordered.push_back((*it)->getCircleCenter());
            T = *it;
            L.removeOne(T);

        }else{
            isClosed = false;
            const Vector2D circumCircle = (T)->getCircleCenter();

            const Vector2D proj =  Vector2D::projection(edge,P, circumCircle);

            qDebug() << (width()-10)/scale+origin.x() << "YOOOOOOOO" << (height()+10)/scale+origin.y();
            const Vector2D inter =   Vector2D::findPerpendicularIntersection(circumCircle, proj, (width()-10)/scale+origin.x(), (height()+10)/scale+origin.y());
            qDebug() << proj;

            qDebug() << "IIIIIIIIIIIIIIIi" <<  inter;

            qDebug() << "Closed";




        }
    }
    // Debug output of the reordered triangles
    qDebug();
    MyPolygon *poly = new MyPolygon(Lordered.size());
    qDebug() << "Nombre de points trouvé : " << QString::number(Lordered.size());
    for (const Vector2D &t : Lordered) {
        poly->addVertex(t);
    };

    // À la fin, on sait si le polygone est fermé
    if (isClosed) {
        qDebug() << "La cellule de Voronoi est FERMÉE";
    } else {
        qDebug() << "La cellule de Voronoi est OUVERTE";
    }

    voronoiObj->addPolygon(poly);
    update();


}

void Canvas::processPoly(){
    for(auto &p: vertices){
        processVoronoi(p);
    }
    processVoronoi(vertices[7]);
}
