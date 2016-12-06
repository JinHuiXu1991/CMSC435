CMSC 435
Jin Hui Xu	
Assignment 1
Description: This program that will render triangles using ray tracing and the barycentric coordinate intersection method.
             The input is in a simple text format called NFF, containing information about the view and objects in the scene.
	     And output an image file with PPM format.

This project contains:
trace.cpp
readNFF.cpp
readNFF.h
vector.cpp
vector.h
rayTracing.cpp
rayTracing.h
Makefile

To run the project:
complie : make
run     : trace < tetra-3.nff
output file should be generated in the same directory as "trace.ppm".

Sources:
textbook chapter 2, 3 and 4
http://www.cplusplus.com/reference/cstdio/fscanf/
http://stackoverflow.com/questions/5742967/why-unsigned-char-for-rgb-pixel-data