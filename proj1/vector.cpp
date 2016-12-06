/*****************************************
** File:    vector.cpp
** Project: CMSC 435 Assignment 1, Fall 2015
** Authoer: Jin Hui Xu
** Date:    9/12/2014
** E-mail:  ac39537@umbc.edu
**
**    This file contains the source code of vector class for assignment 1.
** This program contains the implementations of methods and member functions
** for vector class.
**
*****************************************/
#include"vector.h"
#include <math.h>
#include <stdio.h>
using namespace std;

void subtractVectors(float *output, const float *vector1, const float *vector2)
{
	for (int i = 0; i < 3; i++) 
		output[i] = vector1[i] - vector2[i];	
}

void crossProduct(float *output, const float *vector1, const float *vector2)
{
	output[0] = vector1[1] * vector2[2] - vector1[2] * vector2[1]; 
	output[1] = vector1[2] * vector2[0] - vector1[0] * vector2[2]; 
	output[2] = vector1[0] * vector2[1] - vector1[1] * vector2[0]; 
}

void normalize(float *vector) 
{    
	float d;
	d = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]); 
	if (d != 0.0) 
	{
		vector[0] = vector[0] / d; 
		vector[1] = vector[1] / d; 
		vector[2] = vector[2] / d; 
	}
}

void normalizeVector(float *N, const float *vector1, const float *vector2,
		      const float *vector3)
{
	float v0v1[3], v0v2[3];
	subtractVectors(v0v1, vector2, vector1);
	subtractVectors(v0v2, vector3, vector1);
	crossProduct(N, v0v1, v0v2);
	normalize(N);
}

void multiplayVector(float *output, const float *vector1, const float num)
{
	for (int i = 0; i < 3; i++)
		output[i] = vector1[i] * num;
}

void addVectors(float *output, const float *vector1, const float *vector2)
{
	for (int i = 0; i < 3; i++) 
		output[i] = vector1[i] + vector2[i];	
}



