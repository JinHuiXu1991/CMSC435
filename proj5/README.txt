CMSC 435
Jin Hui Xu	
Assignment 5
Description: This program will performs a simple crowd (Boid) simulation.
	     Boids have three goals:
		Collision Avoidance: avoid collisions with nearby flockmates.
		Velocity Matching: attempt to match velocity with nearby flockmates.
		Flock Centering: attempt to stay close to nearby flockmates.

Base code: I will use instructor's code as a base for this assignment. 

This project contains:
fishtank.cpp
fishtank.H
slVector.cpp
slVector.H
slIO.H
Makefile

To run the project:
complie : make
run     : fishtank

Sources:
assignment 5 webpage
notes on assignment 5 webpage
Boids Pseudocode from http://www.kfish.org/boids/pseudocode.html