# Voronoi-Based Drone Routing Project

## Project Overview

This project involves the creation of a drone routing system using Voronoi diagrams to map and manage drone navigation between server areas. The system adheres to the specifications outlined in the project requirements and includes:

- A GitHub repository with the full implementation.
- Documentation and algorithm explanations.
- A dedicated branch for the Voronoi diagram implementation.

## Initial Problem Statement

The initial data for the project was provided in `config1.json` and `config2.json`. Key features of the problem include:

### Servers and Voronoi Areas:

- Servers are identified by unique names.
- Each server is associated with a colored area defined by a Voronoi diagram.
- Servers are connected through a wired network, with connections existing when server polygons share a common edge.

### Drones:

- Drones are uniquely identified and have an initial position and a target server area.
- Drones communicate only with the server associated with the zone they are flying over.
- Servers guide the drones to their target areas.

## Project Deliverables

The GitHub repository contains:

- The source code, organized and thoroughly commented.
- A dedicated branch for the Voronoi diagram implementation (`voronoi` branch).
- HTML documentation generated using Doxygen.

## Implementation Details

### Voronoi Diagram Construction

The Voronoi diagram forms the backbone of the mapping system, dividing the area into zones associated with each server. This process involves:

#### Input Processing:

- Input consisted of points rather than triangles.
- Points were sorted using a polar angle in ascending order to prepare for convex hull construction.

#### Convex Hull Construction:

- The convex hull was constructed to define the boundary of the area.

#### Triangulation:

- An ear-clipping algorithm was applied to subdivide the convex hull into triangles.

#### Delaunay Triangulation and Flipping:

- Delaunay triangulation was performed using edge-flipping techniques to ensure the properties of the Voronoi diagram were satisfied.

### Drone Routing System

Once the Voronoi diagrams were generated and zones mapped:

#### Integration of Drone Data:

- Data about drones, including their initial positions and target areas, was added to the system.

#### Routing Algorithm:

- Drones were routed through the network of server areas.
- Each server guided the drones within its zone to reach their target areas efficiently.

## Branch Organization

The `voronoi` branch is the exercise of constructing voronoi polygons from triangles.
The `triangulation-convexHull-voronoi` branch is the part where we construct the voronoi polygons from a point cloud.

## Code Quality and Documentation

### Code Comments

The source code is thoroughly commented to explain the purpose and functionality of each component, making it easier to understand and maintain.

### Doxygen Documentation

HTML documentation has been generated using Doxygen and is available in the repository. This documentation provides an overview of the project structure, functions, and classes used in the implementation.
