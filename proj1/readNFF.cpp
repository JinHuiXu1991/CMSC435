/*****************************************
** File:    readNFF.cpp
** Project: CMSC 435 Assignment 1, Fall 2015
** Authoer: Jin Hui Xu
** Date:    9/12/2015
** E-mail:  ac39537@umbc.edu
**
**    This file contains the source code of readNFF class for assignment 1.
** This program contains the implementations of methods and member functions
** for readNFF class.
**
*****************************************/

#include"readNFF.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <queue>
#include <iostream>
using namespace std;

void background(FILE *filename, threeColor background_color)
{
	float r, g, b;
	
	if(fscanf(filename, "%g %g %g", &r, &g, &b) != 3)
	{
		setColor(background_color, 0, 0, 0);
	}
	setColor(background_color, r, g, b);
	

}

void viewing(FILE *filename, viewpoint *view)
{
	float x, y, z;
	threePoint fromToAt;
	float angle;
	float hither;
	int resolutionX, resolutionY;
	
	fscanf(filename, " from %g %g %g", &x, &y, &z);
	setThreePoint(view->from, x, y, z);
	
	fscanf(filename, " at %g %g %g", &x, &y, &z);
	setThreePoint(view->at, x, y, z);

	fscanf(filename, " up %g %g %g", &x, &y, &z);
	setThreePoint(view->up, x, y, z);

	fscanf(filename, " angle %g", &angle);
	view->angle = angle;

	fscanf(filename, " hither %g", &hither);
	view->hither = hither;

	fscanf(filename, " resolution %d %d", &resolutionX, &resolutionY);
	view->resolution[0] = resolutionX;
	view->resolution[1] = resolutionY;

	subtractVectors(fromToAt, view->from, view->at);
	view->aspectRatio = 1.0;

	//calcuate the top, bottom, right and left for computing view ray
	view->t = tan(((angle / 2.0)) * (3.141592653589 / 180.0)) 
			* sqrt(fromToAt[0] * fromToAt[0] + fromToAt[1] * fromToAt[1] + fromToAt[2] * fromToAt[2]);
 	view->b = -view->t;
 	view->r = (view->resolution[0] / view->resolution[1]) * view->t;
 	view->l = -view->r;
}

void filling(FILE *filename, fillobject *newFill)
{
	float r, g, b, kd, ks, shine, T, index_of_refraction;

	if(fscanf(filename, "%g %g %g %g %g %g %g %g", 
		&r, &g, &b, &kd, &ks, &shine, &T, &index_of_refraction) != 8)
	{
		setColor(newFill->fillColor, 0, 0, 0);
	}
	//read only first three input
	setColor(newFill->fillColor, r, g, b);
}

void triangle(FILE *filename, queue<polygon_tri> *triangles)
{
	float x, y, z;
	polygon_tri triangle;
	int verticeNum;

	fscanf(filename, "%d", &verticeNum);

	for(int i = 0; i < verticeNum; i++)
	{
		fscanf(filename, " %g %g %g", &x, &y, &z);
		setThreePoint(triangle.vertices[i], x, y, z);	
	}

	//normalize triangle vectors
	normalizeVector(triangle.normal, triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]);

	//push this triangle into queue
	triangles->push(triangle);
	

}

void readFile(FILE *filename, fillobject *newFill, viewpoint *view, queue<polygon_tri> *triangles, threeColor background_color)
{
	char c;

	while((c = getc(filename)) != EOF)
	{
		if(c == 'b') //read background color data
			background(filename, background_color);
		
		if(c == 'v') //read viewpoint data
			viewing(filename, view);
		
		if(c == 'f') //read fill object data
			filling(filename, newFill);

		if(c == 'p') //read triangles data
			triangle(filename, triangles);

	}
}