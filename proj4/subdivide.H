/*****************************************
** File:    subdivide.H
** Project: CMSC 435 Assignment 4, Fall 2015
** Authoer: Jin Hui Xu
** Date:    11/07/2015
** E-mail:  ac39537@umbc.edu
**
**    This file contains the header of subdivide class for assignment 4.
** This program contains the headers of methods and member functions
** for subdivide class.
**
*****************************************/

#include <vector>
#include "slVector.H"


class Triangle {
  int vertices[3];
public:
  int &operator[] (int i) {return vertices[i];};
};

std::vector<SlVector3> vertices;   // a list of triangle vertices
std::vector<SlVector3> vertexNormals; // a normal for each vertex, for smooth shading
std::vector<SlVector3> faceNormals; // a normal for each face (triangle), for flat shading
std::vector<Triangle> triangles; // a list of triangles
void readObject(char *fname); // read the object from a file
void computeNormals(); // compute normals for the object




