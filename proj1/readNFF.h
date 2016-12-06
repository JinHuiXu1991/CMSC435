/*****************************************
** File:    readNFF.h
** Project: CMSC 435 Assignment 1, Fall 2015
** Authoer: Jin Hui Xu
** Date:    9/12/2015
** E-mail:  ac39537@umbc.edu
**
**    This file contains the header of readNFF class for assignment 1.
** This program contains the headers of methods and member functions
** for readNFF class.
**
*****************************************/

#ifndef READNFF_H
#define READNFF_H
#include"vector.h"
#include <fstream>
#include <math.h>
#include <queue>
using namespace std;

typedef float threePoint[3];
typedef float threeColor[3];

struct viewpoint{
	threePoint from;
	threePoint at;
	threePoint up;
	float angle;
	float hither;
	int resolution[2];
	float aspectRatio, t, b, r, l;
};

struct fillobject{
	threeColor fillColor;
};

struct polygon_tri{
	threePoint vertices[3];
	threePoint normal;
};

#define setColor(i, R, G, B) {i[0] = R; i[1] = G; i[2] = B;}
#define setThreePoint(i, X, Y, Z) {i[0] = X; i[1] = Y; i[2] = Z;}

void background(FILE *filename, threeColor background_color);
void viewing(FILE *filename, viewpoint *view);
void filling(FILE *filename, fillobject *newFill);
void triangle(FILE *filename, queue<polygon_tri> *triangles);
void readFile(FILE *filename, fillobject *newFill, viewpoint *view, 
			  queue<polygon_tri> *triangles, threeColor background_color);

#endif