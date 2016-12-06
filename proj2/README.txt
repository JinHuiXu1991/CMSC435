CMSC 435
Jin Hui Xu	
Assignment 2
Description: This program that will render a object using ray tracing and the barycentric coordinate intersection method, and
	     added mirror reflection and diffuse and Blinn-Phong specular lighting with shadows based one the first assignment.
             The input is in a simple text format called NFF, containing information about the view and objects in the scene.
	     And output an image file with PPM format.

Base code: I will use instructor's code as a base for this assignment.

This project contains:
trace.cpp
trace.H
slVector.cpp
slVector.H
slIO.H
Makefile

To run the project:
complie : make
run     : trace teapot-3.nff output.ppm
output file should be generated in the same directory.

Sources:
assignment 2 webpage
textbook chapter 4 and 13