/**
 * @brief City management class for representing individual cities and their properties.
 * @author Yohann Mitel & Pierre Meyer
 * @date Janvier 2025
 **/
#ifndef CITY_H
#define CITY_H

#include <QString>
#include "vector2d.h"
#include "mypolygon.h"

/**
 * @brief The City class represents a city with a name, position, color, and map data.
 */
class City {
private:
    QString name; ///< Name of the city.
    Vector2D *position = nullptr; ///< Pointer to the position of the city.
    QString color; ///< Color associated with the city.
    MyPolygon *map = nullptr; ///< Pointer to the polygon representing the city's map.

public:
    /**
     * @brief Constructor for the City class.
     * @param _name Name of the city.
     * @param _position Pointer to the city's position.
     * @param _color Color of the city.
     */
    City(QString _name, Vector2D *_position, QString _color);

    /**
     * @brief Destructor for the City class.
     */
    ~City();

    /**
     * @brief Gets the name of the city.
     * @return Name of the city.
     */
    QString getName() const;

    /**
     * @brief Gets the position of the city.
     * @return Pointer to the city's position.
     */
    Vector2D* getPosition();

    /**
     * @brief Gets the color associated with the city.
     * @return Color of the city.
     */
    QString getColor();

    /**
     * @brief Gets the map of the city.
     * @return Pointer to the polygon representing the city's map.
     */
    MyPolygon* getMap();

    /**
     * @brief Sets the map of the city.
     * @param poly Pointer to the polygon representing the map.
     */
    void setMap(MyPolygon *poly);

    /**
     * @brief Draws the city's map on the canvas.
     * @param painter Reference to the QPainter object.
     * @param transparency Flag to enable or disable transparency.
     */
    void drawMap(QPainter &painter, bool transparency);
};

#endif // CITY_H
