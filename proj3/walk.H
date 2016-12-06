/*****************************************
** File:    walk.H
** Project: CMSC 435 Assignment 3, Fall 2015
** Authoer: Jin Hui Xu
** Date:    10/15/2015
** E-mail:  ac39537@umbc.edu
**
**    This file contains the header of walk class for assignment 3.
** This program contains the headers of methods and member functions
** for walk class.
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

class Triangle {
  int vertices[3];
public:
  int &operator[] (int i) {return vertices[i];};
};

// abstract base class for a transformation, the apply() method applies the transform
// to the OpenGL matrix stack
class Transform {
public:
  Transform() {};
  virtual ~Transform() {};
  virtual void apply() const = 0;
};

// Translate
class Translate : public Transform {
  SlVector3 translation;
public:
  Translate(const SlVector3 &_translation) : translation(_translation){};
  void apply() const;
};

// Scale
class Scale : public Transform {
  SlVector3 scale;
public:
  Scale (const SlVector3 &_scale) : scale(_scale){};
  void apply() const;
};

// Rotate
class Rotate : public Transform {
  double angle;
  SlVector3 axis;
public:
  Rotate(double _angle, const SlVector3 &_axis) : axis(_axis), angle(_angle) {};
  void apply() const;
};

// Object class, these are the triangle mesh objects that will be drawn.
class Object {
public:
  std::vector<SlVector3> vertices;   // a list of triangle vertices
  std::vector<SlVector3> vertexNormals; // a normal for each vertex, for smooth shading
  std::vector<SlVector3> faceNormals; // a normal for each face (triangle), for flat shading
  std::vector<Triangle> triangles; // a list of triangles
  std::vector<Transform *> transforms; // transforms for this object
  float ka, kd, ks; // surface reflectance properties
  SlVector3 color; // surface color
  GLuint dlid; // a unique id for the object.  This is useful if you implement display lists
  Object() {}; // constructor
  ~Object() {for (unsigned int i=0; i<transforms.size(); i++) delete transforms[i];}; // destructor
  void readObject(char *fname); // read the object from a file
  void drawObject(); // draw the object
  void computeNormals(); // compute normals for the object
  void createDisplayList(); // create the display list for an object
};

class Light {
public:
  GLfloat ambient[4]; // ambient color, RGBA
  GLfloat diffuse[4]; // diffuse color, RGBA
  GLfloat specular[4]; // specular color, RGBA
  GLfloat pos[4]; // light position, XYZW
  GLenum id; // light identifier
  Light() {}; // constructor
  void apply(); // add the light to the scene
};

class Camera {
public:
  SlVector3 eye; // eye point, center of projection
  SlVector3 center; // point the eye is looking at
  SlVector3 up; // up direction
  float fov, nearplane, farplane; // field of view, distance to near and far clipping planes
};

class Viewport {
public:
  int w, h; // viewport width and height
};

