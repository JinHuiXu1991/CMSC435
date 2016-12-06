/*****************************************
** File:    rayTracing.h
** Project: CMSC 435 Assignment 1, Fall 2015
** Authoer: Jin Hui Xu
** Date:    9/12/2015
** E-mail:  ac39537@umbc.edu
**
**    This file contains the header of rayTracing class for assignment 1.
** This program contains the headers of methods and member functions
** for rayTracing class.
**
*****************************************/

#ifndef RAYTRACING_H
#define RAYTRACING_H

#include"readNFF.h"
#include"vector.h"
using namespace std;

void computeViewingRay(threePoint d, threePoint u, threePoint v, int i, int j, viewpoint *view);
bool rayTri(threePoint r,  threePoint D, polygon_tri *triangle, double t0, double t1);
bool hitObject(threePoint e, threePoint d, queue<polygon_tri> triangles, polygon_tri hitTri,
			   double t0, double t1);
void rayTrace(unsigned char *pixels, queue<polygon_tri> triangles, threeColor background_color,
		viewpoint *view, int width, int height, fillobject *newFill);

#endif