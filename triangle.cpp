#include "triangle.h"
#include <QThread>

void Triangle::computeCircle() {
    Vector2D AB = *ptr[1]-*ptr[0];
    Vector2D AC = *ptr[2]-*ptr[0];
    //OBprim= OA+0.5 AC
    Vector2D OBprim = *ptr[0] + 0.5*AC;
    Vector2D VAC = AC.orthoNormed();

    double k = (AB*AB - AC*AB)/(2*(VAC*AB));

    circumCenter = OBprim + k * VAC;
    circumRadius = (circumCenter-ptr[0]).length();
}

void Triangle::draw(QPainter &painter) {

    QPen pen(Qt::black);
    pen.setWidth(3);
    painter.setPen(pen);

    QColor c= isDelaunay?Qt::cyan:(flippable?Qt::gray:Qt::yellow);
    //painter.setBrush(isHighlited?(isDelaunay?Qt::green:Qt::red):Qt::yellow);

    if(isHighlighted()){
        float h,s,l;
        c.getHslF(&h,&s,&l);
        c.setHslF(h,s,l*0.75);

    }
    painter.setBrush(c);

    QPointF points[3];

    for (int i=0; i<3; i++) {
        points[i].setX(ptr[i]->x);
        points[i].setY(ptr[i]->y);
    }

    painter.drawPolygon(points,3);

}


void Triangle::transparencyDraw(QPainter &painter) {

    QPen pen(QColor(128, 128, 128, 128)); // Gris (128,128,128) avec 50% de transparence (alpha = 128)
    pen.setWidth(3);
    pen.setStyle(Qt::DotLine); // Définit un style de ligne pointillé
    painter.setPen(pen);

    // painter.setBrush(isHighlited ? (isDelaunay ? Qt::green : Qt::red) : Qt::yellow);

    QPointF points[3];

    for (int i = 0; i < 3; i++) {
        points[i].setX(ptr[i]->x);
        points[i].setY(ptr[i]->y);
    }

    painter.drawPolygon(points, 3);

}



void Triangle::drawCircle(QPainter &painter) {
    painter.setPen(QPen(Qt::black,3,Qt::DashLine));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(circumCenter.x-circumRadius,circumCenter.y-circumRadius,2.0*circumRadius,2.0*circumRadius);
}





bool Triangle::flippIt(QVector<Triangle*> triangles) {
    QVector<const Vector2D*> commonEdges;

    qDebug() << "OPPOSITE : " << *(this->getOpposite());
    // Iterate over all neighboring triangles
    for (auto tri : triangles) {
        if (  tri->isFlippable() && tri != this  && tri->contains(this->getOpposite())) {
            // Check each of the 3 edges to find a common edge with the adjacent triangle
            if (tri->hasEdge(this->getVertexPtr(1), this->getVertexPtr(0))) {
                // Common edge is (1, 0) in the current triangle, and the third vertex is (2)

                commonEdges = {getVertexPtr(1), getVertexPtr(0)};

                // Flip both triangles by updating their vertices
                this->updateVertices(
                    this->getOpposite(),
                    const_cast<Vector2D*>(commonEdges[0]),
                    tri->getOpposite()
                    );

                tri->updateVertices(
                    tri->getOpposite(),
                    const_cast<Vector2D*>(commonEdges[1]),
                    this->getOpposite()
                    );

                tri->computeCircle();
                this->computeCircle();
                // Exit loop after flipping both triangles
                return true;  // No need to keep searching, the flip is complete
            } else if (tri->hasEdge(getVertexPtr(2), getVertexPtr(1))) {
                // Common edge is (2, 1) in the current triangle, and the third vertex is (0)

                commonEdges = {getVertexPtr(2), getVertexPtr(1)};

                // Flip both triangles by updating their vertices
                this->updateVertices(
                    this->getOpposite(),
                    const_cast<Vector2D*>(commonEdges[0]),
                    tri->getOpposite()
                    );

                tri->updateVertices(
                    tri->getOpposite(),
                    const_cast<Vector2D*>(commonEdges[1]),
                    this->getOpposite()
                    );


                tri->computeCircle();
                this->computeCircle();
                // Exit loop after flipping both triangles
                return true;  // No need to keep searching, the flip is complete
            } else if (tri->hasEdge(getVertexPtr(0), getVertexPtr(2))) {
                // Common edge is (0, 2) in the current triangle, and the third vertex is (1)

                commonEdges = {getVertexPtr(0), getVertexPtr(2)};

                // Flip both triangles by updating their vertices
                this->updateVertices(
                    this->getOpposite(),
                    const_cast<Vector2D*>(commonEdges[0]),
                    tri->getOpposite()
                    );

                tri->updateVertices(
                    tri->getOpposite(),
                    const_cast<Vector2D*>(commonEdges[1]),
                    this->getOpposite()
                    );

                tri->computeCircle();
                this->computeCircle();
                // Exit loop after flipping both triangles
                return true;  // No need to keep searching, the flip is complete
            }
        }
    }

    return false;


}
