/*****************************************
** File:    fishtank.H
** Project: CMSC 435 Assignment 5, Fall 2015
** Authoer: Jin Hui Xu
** Date:    11/20/2015
** E-mail:  ac39537@umbc.edu
**
**    This file contains the header of fishtank class for assignment 5.
** This program contains the headers of methods and member functions
** for fishtank class.
**
*****************************************/

#include <vector>
#include "slVector.H"
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif


class fish {
  public:
	SlVector3 position;
	SlVector3 velocity;
	int index;
	bool perching;
};

class Viewport {
public:
  int w, h; // viewport width and height
};

