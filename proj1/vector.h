/*****************************************
** File:    vector.h
** Project: CMSC 435 Assignment 1, Fall 2015
** Authoer: Jin Hui Xu
** Date:    9/12/2015
** E-mail:  ac39537@umbc.edu
**
**    This file contains the header of vector class for assignment 1.
** This program contains the headers of methods and member functions
** for vector class.
**
*****************************************/

#ifndef VECTOR_H
#define VECTOR_H
#include <math.h>
#include <stdio.h>


void subtractVectors(float *output, const float *vector1, const float *vector2);
void crossProduct(float *output, const float *vector1, const float *vector2);
void normalize(float *vector);
void normalizeVector(float *N, const float *vector1, const float *vector2,
					 const float *vector3);
void multiplayVector(float *output, const float *vector1, const float num);
void addVectors(float *output, const float *vector1, const float *vector2);

#endif