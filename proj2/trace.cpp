/*****************************************
** File:    trace.cpp
** Project: CMSC 435 Assignment 2, Fall 2015
** Authoer: Jin Hui Xu
** Date:    9/27/2015
** E-mail:  ac39537@umbc.edu
**
**    This file contains main driver and the source code of trace class for assignment 2.
** This program contains the implementations of methods and member functions
** for trace class.
**
*****************************************/


#include "trace.H"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <getopt.h>
#include <math.h>
#ifdef __APPLE__
#define MAX std::numeric_limits<double>::max()
#else
#include <values.h>
#define MAX DBL_MAX
#endif

// return the determinant of the matrix with columns a, b, c.
double det(const SlVector3 &a, const SlVector3 &b, const SlVector3 &c) {
  return a[0]* (b[1] * c[2] - c[1] * b[2]) +
	b[0] * (c[1] * a[2] - a[1] * c[2]) +
	c[0] * (a[1] * b[2] - b[1] * a[2]);
}

inline double sqr(double x) {return x*x;} 

//calculate the intersect of triangle and store its hit record
bool Triangle::intersect(const Ray &r, double t0, double t1, HitRecord &hr) const {
  //normalize triangle vectors
  SlVector3 n = cross(b-a, c-a);
  normalize(n);
  SlVector3 ba = a-b;
  SlVector3 ca = a-c;
  SlVector3 ea = a-r.e;
  double detA = det(ba, ca, r.d);
  double t = det(ba, ca, ea)/detA;
  if (t < t0 || t > t1) return false;
  double beta = det(ea, ca, r.d)/detA;
  if (beta < 0 || beta > 1) return false;
  double gamma = det(ba, ea, r.d)/detA;
  if (gamma < 0.0 || gamma > 1.0-beta) return false;
  hr.t = t;
  hr.p = r.e + t * r.d;
  hr.normal = n;
  return true;
}

//calculate the intersect of sphere and store its hit record
bool Sphere::intersect(const Ray &r, double t0, double t1, HitRecord &hr) const {
  double ddotemc = dot(r.d, r.e-c);
  double d2 = sqrMag(r.d);

  double disc = sqr(ddotemc) - d2 * (sqrMag(r.e-c) - rad*rad);

  if (disc < 0) return false;
  double root1 = (-ddotemc + sqrt(disc)) / d2;
  double root2 = (-ddotemc - sqrt(disc)) / d2;

  double t = root1;
  if (root1 < 0 || (root2 > 0 && root2 < root1)) t = root2;
  if (t < t0 || t > t1) return false;
  
  hr.t = t;
  hr.p = r.e + t * r.d;
  return true;
}

SlVector2 project(const SlVector3 &x, int projectDir) {
  switch (projectDir) {
  case 0:
	return SlVector2(x[1],x[2]);
  case 1:
	return SlVector2(x[0],x[2]);
  case 2:
	return SlVector2(x[0],x[1]);
  }
  return SlVector2(1.0, 1.0);
}

//calculate the intersect of polygon and store its hit record
bool Poly::intersect(const Ray &r, double t0, double t1, HitRecord &hr) const {
  int projectDir;

  //normalize polygon vectors
  SlVector3 n = cross(verts[1]-verts[0], verts[2]-verts[0]);
  normalize(n);

  double t = -(dot(r.e, n) - dot(verts[0], n)) / dot(r.d,n);
  if (t < t0 || t > t1) return false;

  SlVector3 p = r.e + t*r.d;

  if (fabs(n[0]) > fabs(n[1]) && fabs(n[0]) > fabs(n[2])) {
	projectDir = 0;
  } else if (fabs(n[1]) > fabs(n[2])) {
	projectDir = 1;
  } else {
	projectDir = 2;
  }
  
  SlVector2 p2 = project(p, projectDir);

  SlVector2 bbMin = project(verts[0], projectDir);
  SlVector2 bbMax = bbMin;
  for (unsigned int i=1; i<verts.size(); i++) {
	SlVector2 v = project(verts[i], projectDir);
	if (v[0] < bbMin[0]) bbMin[0] = v[0];
	if (v[0] > bbMax[0]) bbMax[0] = v[0];
	if (v[1] < bbMin[1]) bbMin[1] = v[1];
	if (v[1] > bbMax[1]) bbMax[1] = v[1];
  }
  
  if (p2[0] < bbMin[0]) return false;
  if (p2[1] < bbMin[1]) return false;
  if (p2[0] > bbMax[0]) return false;
  if (p2[1] > bbMax[1]) return false;

  SlVector2 dir(sqrt(2), sqrt(2));
  int count = 0;
  for (unsigned int i=0; i<verts.size(); i++) {
	SlVector2 a = project(verts[i], projectDir);
	SlVector2 b = project(verts[(i+1) % verts.size()], projectDir);
	SlVector2 ab = b-a;
	double t2 = cross(a-p2, ab / cross(dir, ab));
	if (t2 < 0.0) continue;
	double alpha = cross(a-p2, dir / cross(dir, ab));
	if (alpha > 0.0 && alpha < 1.0) count++;
  }

  if (count % 2 == 0) {
	return false;
  }

  hr.t = t;
  hr.p = p;
  hr.normal = n;
  return true;
}

//read input file and store the input data
Tracer::Tracer(const std::string &fname) {
  std::ifstream in(fname.c_str(), std::ios_base::in);
  std::string line;
  char ch;
  while (in) {
    getline(in, line);
    switch (line[0]) {
      //read background color data
    case 'b': {
      std::stringstream ss(line);
      ss>>ch>>bcolor[0]>>bcolor[1]>>bcolor[2];
      break;
    }
      
      //read viewpoint data
    case 'v': {
      getline(in, line);
      std::string junk;
      std::stringstream fromss(line);
      fromss>>junk>>eye[0]>>eye[1]>>eye[2];
      
      getline(in, line);
      std::stringstream atss(line);
      atss>>junk>>at[0]>>at[1]>>at[2];
      
      getline(in, line);
      std::stringstream upss(line);
      upss>>junk>>up[0]>>up[1]>>up[2];
      
      getline(in, line);
      std::stringstream angless(line);
      angless>>junk>>angle;
      
      getline(in, line);
      std::stringstream hitherss(line);
      hitherss>>junk>>hither;
      
      getline(in, line);
      std::stringstream resolutionss(line);
      resolutionss>>junk>>res[0]>>res[1];
      break;
    }
      
      //read polygon data
    case 'p': {
      std::stringstream ssn(line);
      unsigned int nverts;
      //check polygon path
      if(line[1] == 'p') {
	ssn>>ch>>ch>>nverts;
      }
      
      ssn>>ch>>nverts;
      std::vector<SlVector3> vertices;
      for (unsigned int i=0; i<nverts; i++) {
	getline(in, line);
	std::stringstream ss(line);
	SlVector3 v;
	ss>>v[0]>>v[1]>>v[2];
	vertices.push_back(v);
      }
      
      //store the polygon and its fill data into surface object list
      if (vertices.size() == 3) {
	surfaces.push_back(std::pair<Surface *, Fill>(new Triangle(vertices[0], vertices[1], vertices[2]), fill));
      } 
      else {
	surfaces.push_back(std::pair<Surface *, Fill>(new Poly(vertices), fill));
      }
      break;
    }
      
      //read sphere data
    case 's' : {
      std::stringstream ss(line);
      SlVector3 c;
      double r;
      ss>>ch>>c[0]>>c[1]>>c[2]>>r;
      surfaces.push_back(std::pair<Surface *, Fill>(new Sphere(c,r), fill));
      break;
    }
      
      //read fill object data
    case 'f' : {
      std::stringstream ss(line);
      ss>>ch>>fill.color[0]>>fill.color[1]>>fill.color[2]>>fill.kd>>fill.ks>>fill.shine>>fill.t>>fill.ior;
      break;
    }
      
      //read light data 
    case 'l' : {
      SlVector3 light;
      std::stringstream ss(line);
      ss>>ch>>light[0]>>light[1]>>light[2];
      lights.push_back(light);
      break;
      
    }
      
    default:
      break;
    }
  }
  im = new SlVector3[res[0]*res[1]];
}

Tracer::~Tracer() {
  if (im) delete [] im;
  for (unsigned int i=0; i<surfaces.size(); i++) delete surfaces[i].first;
}

SlVector3 Tracer::shade(const HitRecord &hr) const {
  return hr.f.color;
}

//ray trace and return the object color for pixel
SlVector3 Tracer::trace(const Ray &r, double t0, double t1, int reflectionLimit) const {
  HitRecord hr, hr2;
  SlVector3 totalColor(0, 0, 0);
  SlVector3 localColor(0, 0, 0);
  SlVector3 L, H, R, p, Lcopy, Dcopy, LDsum;
  bool hit = false;
  bool light_hit = false;
  double diffuse, specular, lightIntensity;

  //algorithm from chapter 4.7
  for (unsigned int k=0; k<surfaces.size(); k++) {
	const std::pair<Surface *, Fill> &s  = surfaces[k];
	if (s.first->intersect(r, t0, t1, hr)) {
	  t1 = hr.t;
	  hr.f = s.second;
	  hit = true;
	}
  }

  if (hit == true) {
    //light intensity of 1/sqrt(numLights)
    lightIntensity = 1 / sqrt(lights.size());
    
    //calculate p
    p = r.e + hr.t * r.d;
    
    //set hit record's fill color to current total color 
    totalColor = hr.f.color * lightIntensity;

    //totalColor = localColor + Ks*reflection + T*refraction
    //check for each light
    for (unsigned int a = 0; a<lights.size(); a++) {
      L = p - lights[a];
      Ray lightRay(p, L);
      
      for (unsigned int k=0; k<surfaces.size(); k++) {
	const std::pair<Surface *, Fill> &x = surfaces[k];
	if (x.first->intersect(lightRay, 0.0001, MAX, hr2)) {
	  light_hit = true;
	}
      }
      
      //if the light isn't blocked by other objects, then continue to compute shadows
      if(light_hit != true){
	
	//calculate H value
	Dcopy = r.d;
	Dcopy = -1 * Dcopy;
	normalize(Dcopy);
	
	Lcopy = L;
	normalize(Lcopy);
	
	// h = normalized(normalized(l) + normalized(-d))
	LDsum = Lcopy + Dcopy;
	H = normalize(LDsum);
	
	//calculate diffuse and specular values
	//diffuse = max(0, dot(N,L))
	if(dot(hr.normal, Lcopy) > 0)
	  diffuse = dot(hr.normal, Lcopy);
	else
	  diffuse = 0;
	
	//specular = pow(max(0, dot(N,H)), e)
	if(dot(H, hr.normal) > 0)
	  specular = pow(dot(H, hr.normal), hr.f.shine);
	else
	  specular = pow(0, hr.f.shine);
	
	//calculate local color
	//localColor.r += (diffuse*Kd*r + Ks*specular) * lightIntensity
	//localColor.g += (diffuse*Kd*g + Ks*specular) * lightIntensity
	//localColor.b += (diffuse*Kd*b + Ks*specular) * lightIntensity
	totalColor += (diffuse * hr.f.kd * totalColor + hr.f.ks * specular) * lightIntensity;
	
	
      }
      
      //compute the reflection color in a recursive process
      if (hr.f.ks > 0 && reflectionLimit > 0){
	
	//compute reflection ray
	//r = d - 2(d . n)n
	R = r.d - 2 * dot(r.d, hr.normal) * hr.normal;
	
	Ray reflectionRay(p, R);
	//totalColor += ks * trace(reflectionRay)
	totalColor += hr.f.ks * trace(reflectionRay, 0.0001, MAX, reflectionLimit - 1);
      }
      
      //compute the refraction color in a recursive process
      if (hr.f.t > 0 && hr.f.ior > 0) {
	double n_f, n_t;
	SlVector3 T;
	n_f = hr.f.ior;
	n_t = fill.ior;
	
	//compute refraction ray
	//t = (n_f(d - n(d.n)))/n_t - n*sqrt(1 - (n_f^2(1 - (d.n)^2))/n_t^2)
	T = (n_f * (r.d - hr.normal * dot(r.d, hr.normal))) / n_t;
	T = T - hr.normal * sqrt(1 - (((n_f * n_f) * (1 - dot(r.d, hr.normal) * dot(r.d, hr.normal))) / (n_t * n_t)));
	
	Ray refractionRay(p, T);
	//totalColor += T * trace(refractionRay)
	totalColor += hr.f.t * trace(refractionRay, 0.0001, MAX, 0);
	
      }
      
      //protect colors from overflow
      for (unsigned int i = 0; i < 3; i++)
	if (totalColor[i] > 1.0)
	  totalColor[i] = 1.0;
    }	
    return totalColor;
    
  }
  //if nothing was hit, return background color
  else
    return bcolor;


  
}

//trace image function to get object color for each pixel
void Tracer::traceImage() {
  // set up coordinate system
  SlVector3 w = eye - at;
  w /= mag(w);
  SlVector3 u = cross(up,w);
  normalize(u);
  SlVector3 v = cross(w,u);
  
  double d = mag(eye - at);
  double h = tan((M_PI/180.0) * (angle/2.0)) * d;
  double l = -h;
  double r = h;
  double b = h;
  double t = -h;

  SlVector3 *pixel = im;

  //loop each pixel and fill color
  for (unsigned int j=0; j<res[1]; j++) {
	for (unsigned int i=0; i<res[0]; i++, pixel++) {
	  double x = l + (r-l)*(i+0.5)/res[0];
	  double y = b + (t-b)*(j+0.5)/res[1];
	  SlVector3 dir = -d * w + x * u + y * v;
	  normalize(dir);
	  Ray r(eye, dir);
	  (*pixel) = trace(r, 0.0, MAX, 5);
	}
  }
}

//writing output
void Tracer::writeImage(const std::string &fname) {
#ifdef __APPLE__
  std::ofstream out(fname, std::ios::out | std::ios::binary);
#else
  std::ofstream out(fname.c_str(), std::ios_base::binary);
#endif
  out<<"P6"<<"\n"<<res[0]<<" "<<res[1]<<"\n"<<255<<"\n";
  SlVector3 *pixel = im;
  char val;
  for (unsigned int i=0; i<res[0]*res[1]; i++, pixel++) {
	val = (unsigned char)(std::min(1.0, std::max(0.0, (*pixel)[0])) * 255.0);
	out.write (&val, sizeof(unsigned char));
	val = (unsigned char)(std::min(1.0, std::max(0.0, (*pixel)[1])) * 255.0);
	out.write (&val, sizeof(unsigned char));
	val = (unsigned char)(std::min(1.0, std::max(0.0, (*pixel)[2])) * 255.0);
	out.write (&val, sizeof(unsigned char));
  }
  out.close();
}

//main driver, read user input file and output file
int main(int argc, char *argv[]) {
  if (argc != 3) {
	std::cout<<"usage: trace input.nff output.ppm"<<std::endl;
	exit(0);
  }	

  Tracer tracer(argv[1]);
  tracer.traceImage();
  tracer.writeImage(argv[2]);
};
