# dynamic_A_Star

Personal Inquiry Project, A* Proof-of-Concept 
Author: Rachel Strohkorb

This project contains a proof-of-concept for utilizing graph theory and the A* algorithm in graphics applications. Instead of physical space, as in pathfinding, the graph is used to represent color space, from black (0,0,0) to white (1,1,1). Particles that traverse the graph are given a random starting point and all head towards the same goal. The color information (particle positions) is then passed to the vertices of a teapot.

To run, you will need Qt to run qmake. You will also need NGL (found here: https://github.com/NCCA/NGL), a graphics library written by Jon Macey for the NCCA at Bournemouth University. This program should work on both Mac and Linux (although Qt has a tendency to crash on Mac when running the GUI for an undetermined reason). 

HOW TO RUN: To run the test suite, build test.pro and execute. The test suite covers testing the graph data structure and teapot construction. To run the main program, build das.pro and execute.

HOW IT WORKS: A more detailed descriptions of what's going on here.

NGLScene is the main handler of this project's functions. It handles the OpenGL context, manages the graph, particle, and teapot objects, and executes actions based on signals from the GUI.

The graph is the foundation of this project, but it's a rather quick-and-dirty setup, so it has several problems. For the sake of speed and ease of setup, the graph only needs a set of points to initialize itself, and will set up edges based on proximity and a 'minimum degree,' meaning that each node in the graph will have a number of edges at least equal to the degree supplied. Edge weights are based on distance between points. 

Because graph setup is based on proximity, there is no guarantee that the graph will be fully connected unless all the points are sufficently equidistant and the degree is sufficiently high. Because this project includes graphs with randomly generated points, it can sometimes crash when switching to one of the Rand-style graphs. This is due to the created graph not being fully connected, and it is a known problem. A better graph initialization process is needed, or some form of cleanup for nodes that are too close together. 

Other graph improvements that could be made: there is currently a way to remove edges, but there is no way to add edges or add/remove nodes. Dynamically updating the graph while the particles are running could create interesting effects that might be worth looking into.

The graph contains an A* method, which uses the A* algorithm to spit out a trail of positions that will lead from the 'start' node to the 'goal' node. This trail does not include the 'start' position.

When running, NGLScene first initializes the graph to one of the provided options (2D grid, 2D rand, 3D grid, and 3D rand). All of these graphs are contained within [0, 1] so that the positions inside it can be used as rgb color data.

The next step is particle creation. Particles spawn in at random nodes in the graph and query the A* algorithm for a path to the universal goal node. They then follow this path, and upon reaching the goal, they are removed. Particles spawn in up to the particle cap, which is determined by the user. When the goal changes (which can occur if 'Randomize Goal' is turned on or whenever the user hits the 'Change Goal' button), a particle will query A* again for a new path, but complete its journey to the next node along its original path before switching to the new one.

When the teapot options are turned on, these particles pass their position data as color data to the vertices of a teapot object. Because the teapot has over 5,000 triangles and I've set a limit of 99 particles, I simply loop over the particle list to provide enough color data for each vertex. 

OTHER IMPROVEMENTS: I could probably spend more time cleaning up this code. Everything needs to be better commented/documented, and naming conventions are nonexistant. Apologies for function names that appear to do the same things.
