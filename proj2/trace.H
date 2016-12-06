/*****************************************
** File:    trace.H
** Project: CMSC 435 Assignment 2, Fall 2015
** Authoer: Jin Hui Xu
** Date:    9/27/2015
** E-mail:  ac39537@umbc.edu
**
**    This file contains the header of trace class for assignment 2.
** This program contains the headers of methods and member functions
** for trace class.
**
*****************************************/


#ifndef TRACE_H
#define TRACE_H

#include "slVector.H"
#include <vector>

class Ray {
public:
  SlVector3 e;
  SlVector3 d;
  Ray(const SlVector3 &_e, const SlVector3 &_d, int _depth = 0) : e(_e), d(_d) {};
};

class Fill {
public: 
  SlVector3 color;
  double kd, ks, shine, t, ior;
};

class HitRecord {
public:
  double t;
  SlVector3 p;
  SlVector3 normal;
  Fill f;
};

class Surface {
public:
  virtual bool intersect(const Ray &r, double t0, double t1, HitRecord &hr) const = 0;
  virtual ~Surface() {};
};

class Triangle : public Surface {
  SlVector3 a,b,c;
public:
  Triangle(const SlVector3 &_a, const SlVector3 &_b, const SlVector3 &_c) : a(_a), b(_b), c(_c) {};
  virtual bool intersect(const Ray &r, double t0, double t1, HitRecord &hr) const;
};

class Poly : public Surface {
  std::vector<SlVector3> verts;
public:
  Poly(const std::vector<SlVector3> &_verts) : verts(_verts) {}; 
  virtual bool intersect(const Ray &r, double t0, double t1, HitRecord &hr) const;
};

class Sphere : public Surface {
  SlVector3 c;
  double rad;
public:
  Sphere(const SlVector3 &_c, double _r) : c(_c), rad(_r) {};
  bool intersect(const Ray &r, double t0, double t1, HitRecord &hr) const;
};

class Tracer {
  SlVector3 bcolor, eye, at, up;
  double angle, hither;
  unsigned int res[2];
  Fill fill;
  std::vector<std::pair<Surface *, Fill> > surfaces;
  std::vector<SlVector3> lights;
  SlVector3 *im;
public:
  Tracer(const std::string &fname);
  ~Tracer();
  void traceImage();  
  SlVector3 trace(const Ray &ray, double t0, double t1, int reflectionLimit) const;
  SlVector3 shade(const HitRecord &hr) const;
  void writeImage(const std::string &fname);
};
	
#endif
