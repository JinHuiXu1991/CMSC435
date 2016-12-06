/*****************************************
** File:    fishtank.cpp
** Project: CMSC 435 Assignment 5, Fall 2015
** Authoer: Jin Hui Xu
** Date:    11/20/2015
** E-mail:  ac39537@umbc.edu
**
**    This file contains main driver and the source code of fishtank class for assignment 5.
** This program contains the implementations of methods and member functions
** for fishtank class.
**
*****************************************/

#include "fishtank.H"
#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>

using namespace std;

#ifndef M_PI
#define	M_PI		3.14159265358979323846	/* pi */
#endif

// Global Variables
std::vector<fish> fishes;
Viewport viewport;
const double RADIUS = 0.015;
const double MIN_DIST = 0.03;
const double MAX_DIST = 0.05;



//Collision Avoidance: avoid collisions with nearby flockmates
SlVector3 rule1(fish f) {

	SlVector3 ca;
	SlVector3 dist;
	double distance;
	int count = 0;

	ca[0] = 0.0;
	ca[1] = 0.0;
	ca[2] = 0.0;

	//check every fish in the vector
	for(int i = 0; i < fishes.size(); i++) {
		//if current fish is not the input fish
		if(fishes[i].index != f.index) {
			//calculate the distance between these two fishes
			dist = fishes[i].position - f.position;
			distance = mag(dist);

			//if the distance is close enough to be a neighbor
			if(distance < MAX_DIST)
				//if the distance is too close to cause collision
				if(distance < MIN_DIST) {
					//substract that distance to avoid collision
					ca = ca - dist;	
					count++;

				}

		}
	}

	//calculate the average avoidance distance
	ca = ca / (count * 1.0);
	return ca;
}

//Velocity Matching: attempt to match velocity with nearby flockmates
SlVector3 rule2(fish f) {

	SlVector3 vm;
	SlVector3 dist;
	double distance;
	int count = 0;

	vm[0] = 0.0;
	vm[1] = 0.0;
	vm[2] = 0.0;

	//check every fish in the vector
	for(int i = 0; i < fishes.size(); i++) {
		//if current fish is not the input fish
		if(fishes[i].index != f.index) {
			//calculate the distance between these two fishes
			dist = fishes[i].position - f.position;
			distance = mag(dist);

			//if the distance is close enough to be a neighbor
			if(distance < MAX_DIST) {
				//sum the neighbor velocity
				vm = vm + f.velocity;
				count++;
			}

		}
	}
	
	//calculate the average neighbor velocity
	vm = vm / (count * 1.0);
	vm = (vm - f.velocity) * 0.01;
	return vm;

}

//Flock Centering: attempt to stay close to nearby flockmates
SlVector3 rule3(fish f) {

	SlVector3 fc;
	SlVector3 dist;
	double distance;
	int count = 0;

	fc[0] = 0.0;
	fc[1] = 0.0;
	fc[2] = 0.0;

	//check every fish in the vector
	for(int i = 0; i < fishes.size(); i++) {
		//if current fish is not the input fish
		if(fishes[i].index != f.index) {
			//calculate the distance between these two fishes
			dist = fishes[i].position - f.position;
			distance = mag(dist);

			//if the distance is close enough to be a neighbor
			if(distance < MAX_DIST) {
				//sum up the position of neighbors
				fc = fc + f.position;
				count++;
			}
		}
	}

	//calculate the average position of neighbors
	fc = fc / (count * 1.0);
	fc = (fc - f.position) * 0.01;
	return fc;

}

//Extra Rule: add a wind in a direction which influence the velocity of fish
SlVector3 rule4() {
	SlVector3 wind;
	
	wind[0] = 0.01;
	wind[1] = 0.02;
	wind[2] = 0.03;

	return wind;

}

//Boids are constrained to a box from [-0.5,0.5]x[-0.25,0.25]x[-0.125,0.125] 
//(when they leave the box, flip their velocity).
void bound_position(fish f) {


	int Xmin, Xmax, Ymin, Ymax, Zmin, Zmax;
	
	Xmin = -0.5;
	Xmax = 0.5;
	Ymin = -0.25;
	Ymax = 0.25;
	Zmin = -0.125;
	Zmax = 0.125;


	if(f.position[0] < Xmin || f.position[0] > Xmax)
		f.velocity[0] = -f.velocity[0];

	if(f.position[1] < Ymin || f.position[1] > Ymax)
		f.velocity[1] = -f.velocity[1];

	if(f.position[2] < Zmin || f.position[2] > Zmax)
		f.velocity[2] = -f.velocity[2];

}


//limit the maximum velocity
void velocityLimit(fish f) {
	int limit = 0.5;

	if(mag(f.velocity) > limit)
		f.velocity = f.velocity / mag(f.velocity) * limit;

}

//calculate the new position for each fish for every frame
void updatePositions() {
	/*
		Vector v1, v2, v3 ......
		Boid b

		For each fish f
			v1 = rule1(f)
			v2 = rule3(f)
			v3 = rule2(f)
			......

			f.velocity = f.velocity + v1 + v2 + v3 ......
			f.position = f.position + f.velocity
		End
	*/

	SlVector3 v1, v2, v3, v4;
	fish f;

	for(int i = 0; i < fishes.size(); i++) {
		f = fishes[i];

		//multiply the velocity by 0.999 every timestep
		f.velocity = f.velocity * 0.999;

		//apply rules
		v1 = rule1(f);
		v2 = rule2(f);
		v3 = rule3(f);
		v4 = rule4();
		
		//calculate new velocity
		f.velocity = f.velocity + v1 + v2 + v3 + v4;
		
		velocityLimit(f);

		//check boundary
		bound_position(f);
		
		//calculate new position
		f.position = f.position + f.velocity;

		fishes[i].position = f.position;
		fishes[i].velocity = f.velocity;

	}
}


//initialize all fishes with position, velocity and index
void initializeFish() {
	fish f;

	srand(time(NULL));

	//start with 50 fishes
	for(int i = 0; i < 50; i++) {
		//store the fish index
		f.index = i;
		f.perching = false;

		//generate random position based on the x, y and z boundary 
		f.position[0] = (rand() % 11 + (-5)) / 10.0;
		f.position[1] = (rand() % 51 + (-25)) / 100.0;
		f.position[2] = (rand() % 251 + (-125)) / 1000.0;

		//generate random velocity
		f.velocity[0] = (rand() % 50) / 1000;
		f.velocity[1] = (rand() % 50) / 1000;;
		f.velocity[2] = (rand() % 50) / 1000;;

		//push certain fish into vector
		fishes.push_back(f);
	}
}


void drawFish() {
	
	//set up fish color to green
	glClear(GL_COLOR_BUFFER_BIT);
    	glColor3f(0,1,0);


	//draw all fishes
	for(int i = 0; i < fishes.size(); i++) {
		
		glPushMatrix();
		  //draw the fish based on its position
		  glTranslated(fishes[i].position[0], fishes[i].position[1], fishes[i].position[2]);
		  glRotated(90, fishes[i].velocity[0], fishes[i].velocity[1], fishes[i].velocity[2]);
		  glutSolidSphere(RADIUS,10,10);
		  glutSolidCone(RADIUS, 0.05, 10, 10);
		glPopMatrix();
		

	}
	glutSwapBuffers();

	//after drawing all fishes for current frame, update all the positions
	updatePositions();



}


void myReshape(int w, int h) {
  glViewport (0,0,w,h);
  viewport.w = w;
  viewport.h = h;
}

void myDisplay() {
  int i;
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST); /* enable depth testing; required for z-buffer */
  glEnable(GL_CULL_FACE); /* enable polygon face culling */ 
  glCullFace(GL_BACK); /* tell opengl to cull the back face*/ 
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, ((float)viewport.w)/viewport.h, .75, 2.5);
  gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  //apply a single light
  GLfloat pos[4] = {0, 1, 0, 1};
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glEnable(GL_LIGHT0);


  drawFish();
  
  glutSwapBuffers();
}


void myTimerFunc(int id) {
  glutPostRedisplay();
  glutTimerFunc(100, myTimerFunc, 0);
}


int main(int argc, char *argv[]) {

  glutInit(&argc, argv);
  
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(640, 400);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);
  
  //initialize fishes 
  initializeFish(); 

  glutDisplayFunc(myDisplay);
  glutReshapeFunc(myReshape);
  glutTimerFunc(100, myTimerFunc, 0);
  
  glutMainLoop();
  
  return 0;
}
