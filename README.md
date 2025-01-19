# Voronoi Maps and Drones Project in C++

## Project Description

This project aims to generate Voronoi maps using a set of points through a series of algorithmic steps. Each Voronoi cell is represented by a polygon. Simultaneously, drones are integrated, with their positions and destinations extracted from a JSON file. Drones are assigned to specific zones on the generated map, and their movement is simulated until they reach their respective destinations.

## Features

- **Point Cloud Reading**: Points are extracted from a JSON file to define the Voronoi cells.
- **Voronoi Map Generation**: After processing the points, Voronoi polygons representing the map are generated.
- **Drone Integration**: Drones are also retrieved from a JSON file, each with a destination to reach on the map.
- **Drone and Voronoi Zone Processing**: Drones are assigned to specific zones, and their movements are simulated until they reach their destination.

## Process Steps

1. **Voronoi Circumcircle**  
   Calculation of the circumcircles of the Voronoi triangles, used for triangulation management.

2. **Point Cloud**  
   Creation of a set of random or provided points to initiate the map generation.

3. **Polar Angle Sorting**  
   Sorting the points by their polar angle to facilitate the generation of the Voronoi map.

4. **Convex Hull**  
   Calculation of the convex hull of the points to define the map's boundaries.

5. **Triangulation**  
   Using a triangulation algorithm to organize the points before constructing the Voronoi polygons.

6. **Delaunay + Flip**  
   Applying the Delaunay algorithm to ensure optimal triangles, followed by edge flipping to improve quality.

7. **Voronoi (bounded by background) + Neighbor Matrix (polygons)**  
   Generation of Voronoi polygons that respect the canvas boundaries, and creation of a neighbor matrix for each cell.

8. **Drone Code Integration**  
   Integration of drones into the Voronoi model, allowing them to interact with the generated geographic zones.

9. **Drone Scaling**  
   Adjusting the size and movements of drones based on Voronoi zones.

10. **Triangulation within Voronoi**  
    Updating triangulation according to Voronoi zones.

11. **Edge Nearest for Drones Outside Boundaries**  
    Calculating the nearest edges for drones located outside the Voronoi map boundaries.
