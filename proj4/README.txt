CMSC 435
Jin Hui Xu	
Assignment 4
Description: The assignment is to write a program that performs Loop subdivision using a half-edge data 
	    structure. Your program will read a file in a simplified obj format and write an obj file 
	    with the subdivided mesh.

Base code: I will use instructor's code as a base for this assignment.

Comments: The subdiv function in the program has bugs that I couldn't figure out the correct solution by the due day, and 
	 bugs can make the program ends with segmentation faults, so I have commented out the section of subdiv function 
	 in the project for submission for the purpose of testing other functions, and the code inside subdiv function can be tested 
	 by removing the "/*" in the head of function and "*/" in the end of the function. 

This project contains:
subdivide.cpp
subdivide.H
slVector.cpp
slVector.H
slIO.H
Makefile

To run the project:
complie : make
run     : subdivide input.obj output.obj number_of_subdivision_steps

Sources:
assignment 4 webpage
Loop subdivision and half edge structure notes on assignment 4 webpage
Algorithms from webpage: http://www.ugrad.cs.ubc.ca/~cs424/Assignments/a7_prog.pdf