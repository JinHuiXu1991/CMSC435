/*****************************************
** File:    trace.cpp
** Project: CMSC 435 Assignment 1, Fall 2015
** Authoer: Jin Hui Xu
** Date:    9/12/2015
** E-mail:  ac39537@umbc.edu
**
**    This file contains the main driver program for assignment 1.
** 
**
*****************************************/

#include"readNFF.h"
#include"vector.h"
#include"rayTracing.h"
#include <queue>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) 
{
	
	int width, height;
	fillobject newFill;
	viewpoint view;
	queue<polygon_tri> triangles;
	threeColor background_color = {0.0, 0.0, 0.0};

	//parsing input files
	readFile(stdin, &newFill, &view, &triangles, background_color);
	
	width = view.resolution[0];
	height= view.resolution[1];


	unsigned char pixels[height * width * 3];
	
	//ray tracing the triangle
	rayTrace(pixels, triangles, background_color, &view, width, height, &newFill);
	

	return 0;
}