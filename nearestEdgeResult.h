#ifndef NEARESTEDGERESULT_H
#define NEARESTEDGERESULT_H

/**
 * @brief Represents the result of finding the nearest edge.
 *
 * This structure contains the information about the nearest edge from a given point,
 * including the distance to the edge and the identifier of the nearest edge.
 */
struct NearestEdgeResult {
    /**
     * @brief The distance to the nearest edge.
     *
     * This value represents the shortest distance from the reference point to the nearest edge.
     */
    double distance;

    /**
     * @brief The index of the nearest edge.
     *
     * This integer represents the ID or index of the nearest edge relative to the reference point.
     */
    int nearest;
};

#endif // NEARESTEDGERESULT_H
