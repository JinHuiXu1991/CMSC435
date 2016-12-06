/*****************************************
** File:    rayTracing.cpp
** Project: CMSC 435 Assignment 1, Fall 2015
** Authoer: Jin Hui Xu
** Date:    9/12/2015
** E-mail:  ac39537@umbc.edu
**
**    This file contains the source code of rayTracing class for assignment 1.
** This program contains the implementations of methods and member functions
** for rayTracing class.
**
*****************************************/

#include "rayTracing.h"
#include"readNFF.h"
#include"vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

//formulas are from chapter 4.3 
void computeViewingRay(threePoint d, threePoint u, threePoint v, int i, int j, viewpoint *view)
{

	double small_u, small_v;
	threePoint uU, vV, s;

	//u = l + (r − l)(i + 0.5)/nx
	small_u = view->l + (view->r - view->l) * ((double)i + 0.5) / view->resolution[0];

	//v = b + (t − b)(j + 0.5)/ny,
	small_v = view->b + (view->t - view->b) * ((double)j + 0.5) / view->resolution[1];

	//s = e + u U + v V
	multiplayVector(uU, u, small_u);
	multiplayVector(vV, v, small_v);
	addVectors(s, uU, vV);
	addVectors(s, view->from, s);

	//p(t) = e + t(s - e)
	subtractVectors(d, s, view->from);
}

//formulas and algorithm are from chapter 4.4
bool rayTri(threePoint E, threePoint D, polygon_tri *triangle, double t0, double t1) 
{
	double a, b, c, d, e, f, g, h, i, j, k, l;
	double ei_minus_hf, gf_minus_di, dh_minus_eg;
	double ak_minus_jb, jc_minus_al, bl_minus_kc;
	double beta, gamma;
	double t, M;

	//create matrix
	//first row
	a = triangle->vertices[0][0] - triangle->vertices[1][0];
	d = triangle->vertices[0][0] - triangle->vertices[2][0];
	g = D[0];
	j = triangle->vertices[0][0] - E[0];

	//second row
	b = triangle->vertices[0][1] - triangle->vertices[1][1];
	e = triangle->vertices[0][1] - triangle->vertices[2][1];
	h = D[1];
	k= triangle->vertices[0][1] - E[1];

	//third row
	c = triangle->vertices[0][2] - triangle->vertices[1][2];
	f = triangle->vertices[0][2] - triangle->vertices[2][2];
	i = D[1];
	l = triangle->vertices[0][2] - E[2];

	ei_minus_hf = e*i - h*f;
	gf_minus_di = g*f - d*i;
	dh_minus_eg = d*h - e*g;
	ak_minus_jb = a*k - j*b;
	jc_minus_al = j*c - a*l;
	bl_minus_kc = b*l - k*c;
	
	M = a * ei_minus_hf + b * gf_minus_di + c * dh_minus_eg;
	t = f * ak_minus_jb + e * jc_minus_al + d * bl_minus_kc / M;
	if (t < t0 || t > t1)
		return false;

	gamma = i * ak_minus_jb + h * jc_minus_al + g * bl_minus_kc / M;
	if (gamma < 0.0 || gamma > 1.0)
		return false;

	beta = j * ei_minus_hf + k * gf_minus_di + l * dh_minus_eg / M;
	if (beta < 0.0 || beta > (1.0 - gamma))
		return false;

	t1 = t;
	return true;
}


//algorithm are from chapter 4.4.4
bool hitObject(threePoint e, threePoint d, queue<polygon_tri> triangles, polygon_tri hitTri,
			   double t0, double t1)
{
	bool hit = false;
	polygon_tri triTemp;
	polygon_tri EmptyStruct = {};
	queue<polygon_tri> tri;
	queue<polygon_tri> tri2;
	
	//reset hitTri to empty
	hitTri = EmptyStruct;

	//copy queue
	while(!triangles.empty())
	{
		tri.push(triangles.front());
		tri2.push(triangles.front());
		triangles.pop();
	}
	
	//backup original queue
	while(!tri2.empty())
	{
		triangles.push(tri2.front());
		tri2.pop();
	}

	while(!tri.empty())
	{
		triTemp = tri.front();
		if(rayTri(e, d, &triTemp, t0, t1) == true)
		{
			hit = true;
			hitTri = triTemp;
		}
		tri.pop();
	}

	return hit;
}


void rayTrace(unsigned char *pixels, queue<polygon_tri> triangles, threeColor background_color,
			  viewpoint *view, int width, int height, fillobject *newFill)
{
	double t0 = 0.0;
	double t1 = 9999999999;
	threePoint u, v, w, d;
	polygon_tri hitTri;

	//calculate basis vector
	//w = -ray direction
	subtractVectors(w, view->from, view->at);
	normalize(w);

	//u = t X w / |t X w|
	crossProduct(u, view->up, w);
	normalize(u);

	//v = w X u
	crossProduct(v, w, u);
	normalize(v);

	//loop each pixel and fill color
	for (int i = 0; i < width; i++) 
	{
		for (int j = 0; j < height; j++) 
		{
			computeViewingRay(d, u, v, i, j, view);
			//find first object hit by ray and its surface normal n
			if(hitObject(view->from, d, triangles, hitTri, t0, t1) == true)
			{
				pixels[((i+(j*width))*3)+0] = newFill->fillColor[0]*255;
				pixels[((i+(j*width))*3)+1] = newFill->fillColor[1]*255;
				pixels[((i+(j*width))*3)+2] = newFill->fillColor[2]*255;
			} 
			else {
				pixels[((i+(j*width))*3)+0] = background_color[0]*255;
				pixels[((i+(j*width))*3)+1] = background_color[1]*255;
				pixels[((i+(j*width))*3)+2] = background_color[2]*255;
			}
		}

	}
	
	//writing output
	FILE *f = fopen("trace.ppm","wb");
	fprintf(f, "P6\n%d %d\n%d\n", width, height, 255);
	fwrite(pixels, 1, height*width*3, f);
	fclose(f);
	
}
