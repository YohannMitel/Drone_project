#include "cities.h"

#include <queue>

using namespace std;

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

bool Cities::sortingByPointsRelative(const City* city1, const City* city2, const QVector<QPair<QString, Vector2D>>& pointsRelative) {
    // Trouver les indices des villes dans pointsRelative
    int index1 = -1, index2 = -1;

    // Chercher l'indice de city1 et city2 dans pointsRelative en comparant les noms des villes
    for (int i = 0; i < pointsRelative.size(); ++i) {
        if (pointsRelative[i].first == city1->getName() ) {
            index1 = i;
        }
        if (pointsRelative[i].first == city2->getName()) {
            index2 = i;
        }
    }

    // Comparer les indices pour définir l'ordre
    return index1 < index2;
}

QVector<QPair<QString,Vector2D>> Cities::ascendingPolarAngle(Vector2D &porigin) {

        // qDebug() << porigin;
    QVector<QPair<QString,Vector2D>> pointsRelative;
    for (auto &v:tabCities) {
        auto pOrig = *(v->getPosition());

        pointsRelative.append(qMakePair(v->getName(),Vector2D(pOrig.x - porigin.x, pOrig.y - porigin.y) ));
    }
    // sorting point with angular criteria
    qDebug() << "AVANT TRIAGE : " ;
    for(auto &t: pointsRelative){
        qDebug() << t;
    }

    qDebug() << "APRES TRIAGE : " ;
    std::sort(pointsRelative.begin(), pointsRelative.end(),Vector2D::polarComparison);

    for(auto &t: pointsRelative){
        qDebug() << t;
    }


    return pointsRelative;

}



void  Cities::orderPolygonPoint(Vector2D &porigin){
    auto pointsRelativeList = ascendingPolarAngle(porigin);
    int N,Nmax;
    delete convexHull;

    std::sort(tabCities.begin(), tabCities.end(),
              [&pointsRelativeList](City* city1, City* city2) {
                  return sortingByPointsRelative(city1, city2, pointsRelativeList);
              }
              );

    /*for(auto &t: tabCities){
        qDebug() << t->getName();
    }*/


    QVector<Vector2D*> tabVertices = this->getTabVertices();


    QVector<Vector2D> CHstack;
    Vector2D top, top_1;

    CHstack.append(tabVertices[0]);
    CHstack.append(tabVertices[1]);
    CHstack.append(tabVertices[2]);
    auto it=tabVertices.begin()+3;

    while (it!=tabVertices.end()) {
        top = CHstack.last(); // extract top and top_1
        CHstack.pop_back();
        // from the stack

        top_1 = CHstack.last();
        CHstack.push_back(top);


        while (!isOnTheLeft((*it),top_1,top)) {

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
        const Vector2D v = CHstack.first();
        qDebug() << v;
        convexHull->addVertex(v);
        CHstack.pop_front();
    }

    /*std::sort(tabCities.begin(), tabCities.end(),
              [&pointsRelativeList](City* city1, City* city2) {
                  return sortingByPointsRelative(city1, city2, pointsRelativeList);
              }
              );*/

}

QVector<Triangle*> Cities::initTriangulation(){

    QVector<Vector2D *> tabVert = getTabVertices() ;

    return  convexHull->earClipping(tabVert );;
}

bool onSegment(Vector2D p1, Vector2D p2, Vector2D q) {
    return (q.x >= std::min(p1.x, p2.x) && q.x <= std::max(p1.x, p2.x) &&
            q.y >= std::min(p1.y, p2.y) && q.y <= std::max(p1.y, p2.y));
}


void Cities::connectionMatrix(const QVector<City*>& cities) {
    int numCities = cities.size();

    // Create a matrix of size numCities x numCities and initialize to 0
    adjacencyMatrix = QVector<QVector<int>>(numCities, QVector<int>(numCities, 0));

    // Loop over each city to check for neighbors
    for (int i = 0; i < numCities; ++i) {
        City* city1 = cities[i];
        // Get the vertices of city1's Voronoi polygon
        int numVertices1 = 0;
        Vector2D* vertices1 = city1->getMap()->getVertices(numVertices1);

        for (int j = i + 1; j < numCities; ++j) {
            City* city2 = cities[j];
            // Get the vertices of city2's Voronoi polygon
            int numVertices2 = 0;
            Vector2D* vertices2 = city2->getMap()->getVertices(numVertices2);

            // Check if the Voronoi polygons of city1 and city2 share an edge
            if (areNeighbors(vertices1, numVertices1, vertices2, numVertices2)) {
                // Set the matrix entries to 1 to indicate that these cities are neighbors
                adjacencyMatrix [i][j] = 1;
                adjacencyMatrix [j][i] = 1;  // The matrix is symmetric, so we set both [i][j] and [j][i]
            }
        }
    }

    // Optionally, print the matrix for debugging purposes
    for (int i = 0; i < numCities; ++i) {
        qDebug() << "City: " << cities[i]->getName();  // Print city name before the row
        for (int j = 0; j < numCities; ++j) {
            qDebug() << adjacencyMatrix [i][j] << " " << cities[j]->getName();
        }
        qDebug() << "\n";  // Print a new line after each row
    }
}

bool Cities::areNeighbors(Vector2D* vertices1, int numVertices1, Vector2D* vertices2, int numVertices2) {
    // Check if two Voronoi polygons share an edge by checking for intersection between edges.
    for (int i = 0; i < numVertices1; ++i) {
        Vector2D p1 = vertices1[i];
        Vector2D p2 = vertices1[(i + 1) % numVertices1];  // Next vertex (with wrap around)

        for (int j = 0; j < numVertices2; ++j) {
            Vector2D q1 = vertices2[j];
            Vector2D q2 = vertices2[(j + 1) % numVertices2];  // Next vertex (with wrap around)

            // Check if the edge p1p2 intersects with q1q2
            if (doIntersect(p1, p2, q1, q2)) {
                return true;  // Return true if we find an intersection
            }
        }
    }
    return false;  // No intersection found
}

// Helper function to check if two line segments (p1p2 and q1q2) intersect
bool Cities::doIntersect(Vector2D p1, Vector2D p2, Vector2D q1, Vector2D q2) {
    int o1 = orientation(p1, p2, q1);
    int o2 = orientation(p1, p2, q2);
    int o3 = orientation(q1, q2, p1);
    int o4 = orientation(q1, q2, p2);

    // General case
    if (o1 != o2 && o3 != o4) {
        return true;
    }

    // Special cases: Check if the points are collinear and on the segment
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
    if (o2 == 0 && onSegment(p1, p2, q2)) return true;
    if (o3 == 0 && onSegment(q1, q2, p1)) return true;
    if (o4 == 0 && onSegment(q1, q2, p2)) return true;

    return false;  // No intersection
}

// Function to calculate the orientation of the triplet (p, q, r)
int Cities::orientation(Vector2D p, Vector2D q, Vector2D r) {
    // Cross product of vector pq and qr
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // collinear
    return (val > 0) ? 1 : 2;  // 1 -> clockwise, 2 -> counterclockwise
}

QVector<int> findPath(const QVector<QVector<int>>& matrix, int start, int end) {
    int n = matrix.size();
    vector<bool> visited(n, false);  // To track visited cities
    vector<int> parent(n, -1);       // To reconstruct the path
    queue<int> queue;                // Queue for BFS

    // Start BFS from the starting city
    queue.push(start);
    visited[start] = true;

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        // If we reached the destination, reconstruct and return the path
        if (current == end) {
            QVector<int> path;
            for (int at = end; at != -1; at = parent[at]) {
                path.push_back(at);
            }
            reverse(path.begin(), path.end());  // Reverse to get the correct order
            return path;
        }

        // Explore neighbors
        for (int neighbor = 0; neighbor < n; ++neighbor) {
            if (matrix[current][neighbor] == 1 && !visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                queue.push(neighbor);
            }
        }
    }

    // If no path is found, return an empty vector
    return {};
}

void Cities::testPathFinding(int start, int end) {
    QVector<int> path = findPath(adjacencyMatrix, start, end);

    if (path.empty()) {
        qDebug() << "No path found between city" << start << "and city" << end;
    } else {
        qDebug() << "Path from city" << start << "to city" << end << ":";
        for (int city : path) {
            qDebug() << city;  // Or use city names if needed
        }
    }
}
