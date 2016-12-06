/*****************************************
** File:    walk.cpp
** Project: CMSC 435 Assignment 3, Fall 2015
** Authoer: Jin Hui Xu
** Date:    10/15/2015
** E-mail:  ac39537@umbc.edu
**
**    This file contains main driver and the source code of walk class for assignment 3.
** This program contains the implementations of methods and member functions
** for walk class.
**
*****************************************/

#include "walk.H"
#include <iostream>
#include <fstream>
#include <cstring>
#include <time.h>

using namespace std;

#ifndef M_PI
#define	M_PI		3.14159265358979323846	/* pi */
#endif

// Global Variables
std::vector<Object> objects;
std::vector<Light> lights;
Camera camera, origCamera;
Viewport viewport;
bool SMOOTH_SHADING = true;
bool FLAT_SHADING = false;
bool WIREFRAME = false;
float angle;

///////////////////////////////////////////////////
// Begin Class Function Definitions
///////////////////////////////////////////////////

void Translate::apply() const {
  // apply a translation to the OpenGL modelview matrix stack
  // see glTranslated
  glTranslated(translation[0], translation[1], translation[2]);
};

void Scale::apply() const {
  // apply a scale transformation to the OpenGL modelview matrix stack
  // see glScaled
  glScaled(scale[0], scale[1], scale[2]);
};

void Rotate::apply() const {
  // apply a rotation to the OpenGL modelview matrix stack
  // see glRotated
  glRotated(angle, axis[0], axis[1], axis[2]);
};

void Object::readObject(char *fname) {
  std::ifstream in(fname, std::ios::in);
  char c;
  SlVector3 pt;
  Triangle t;
  
  if (!in.good()) {
	std::cerr<<"Unable to open file \""<<fname<<"\""<<std::endl;
	abort();
  }
  
  while (in.good()) {
	in >> c;
	if (!in.good()) break;
	if (c == 'v') {
	  in >> pt[0] >> pt[1] >> pt[2];
	  vertices.push_back(pt);
	} else if (c == 'f') {
	  in >> t[0] >> t[1] >> t[2];
	  t[0]-=1; t[1]-=1; t[2]-=1;
	  triangles.push_back(t);
	}
  }
  // we've got the geometry, lets compute the normals
  computeNormals();
}

void Object::createDisplayList() {
  // If you choose to use display lists, this would be a reasonable place to add that code,
  // otherwise you can just leave this blank
}

void Object::drawObject() {
  std::cout<<"I want to draw object "<<dlid<<" "<<std::endl;
  // here you draw the object
  // you will need to set material properties
  // and normals as well as actually draw the triangles
  // this is also where you call apply transformations
  // remember to push and pop your matrices
  
  //calculate the material properties
  GLfloat am[4] = {ka*color[0], ka*color[1], ka*color[2], 1.0};
  GLfloat di[4] = {kd*color[0], kd*color[1], kd*color[2], 1.0};
  GLfloat sp[4] = {ks*color[0], ks*color[1], ks*color[2], 1.0};
  
  //draw every triangle in the object
  for(int k = 0; k < triangles.size(); k++) {
    glPushMatrix();
    //set the material properties
    glDisable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, am);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, di);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sp); 
    //apply transforms before drawing
    for (int i = 0; i < transforms.size(); i ++) {
      transforms[i]->apply();
    }

    //choose correct normals base on the shading mode 
    if(FLAT_SHADING == true) {
      glNormal3f(faceNormals[k][0], faceNormals[k][1], faceNormals[k][2]);
    }
    else if (SMOOTH_SHADING == true){
      glNormal3f(vertexNormals[k][0], vertexNormals[k][1], vertexNormals[k][2]);
    }
    else {
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }
    
    //start drawing;
    glBegin(GL_POLYGON);
      glVertex3f(vertices[triangles[k][0]][0], vertices[triangles[k][0]][1], vertices[triangles[k][0]][2]);
      glVertex3f(vertices[triangles[k][1]][0], vertices[triangles[k][1]][1], vertices[triangles[k][1]][2]);
      glVertex3f(vertices[triangles[k][2]][0], vertices[triangles[k][2]][1], vertices[triangles[k][2]][2]);
    glEnd();
    glPopMatrix();
  }
  
  
  
}

void Object::computeNormals() {
  // here you compute normals
  // you should compute both face normals (cross product) for flat shading
  // and vertex normals (area-weighted averages of face normals) for
  // smooth shading
  // remember to normalize your normals

  //compute face normals
  for(int i = 0; i < triangles.size(); i++) {
    SlVector3 n = cross(vertices[triangles[i][1]] - vertices[triangles[i][0]], vertices[triangles[i][2]] - vertices[triangles[i][0]]);
    normalize(n);
    faceNormals.push_back(n);
  }
  
  //compute vertex normals
  for(int i = 0; i < vertices.size(); i++) {
    SlVector3 sum;
    int num = 0;
    for(int j = 0; j < triangles.size(); j++) {
      if(triangles[j][0] == i || triangles[j][1] == i || triangles[j][2] == i) {
	sum += faceNormals[j];
	num ++;
      }
    }
    sum = sum / float(num);
    normalize(sum);
    vertexNormals.push_back(sum);
  }

}

void Light::apply() {
  std::cout<<"Applying light "<<id<<std::endl;
  // this function tells openGL about the light
  // it is called from myDisplay().
  // Use glLightfv to set the different properties
  // of the light and glEnable() to turn the light on.
  
  GLfloat ambientIntensity[4] = {ambient[0], ambient[1], ambient[2], ambient[3]};
  GLfloat diffuseIntensity[4] = {diffuse[0], diffuse[1], diffuse[2], diffuse[3]};
  GLfloat specularIntensity[4] = {specular[0], specular[1], specular[2], specular[3]};
  GLfloat position[4] = {pos[0], pos[1], pos[2], pos[3]};
  
  glLightfv(id, GL_AMBIENT, ambientIntensity);
  glLightfv(id, GL_DIFFUSE, diffuseIntensity);
  glLightfv(id, GL_SPECULAR, specularIntensity);
  glLightfv(id, GL_POSITION, position);

  glEnable(id);
		
}

///////////////////////////////////////////////////
// End Class Definitions
///////////////////////////////////////////////////

// this function reads the input file
void parseInputFile(char *fname) {
  std::ifstream in(fname, std::ios::in);
  Object obj;
  char str[80];
  int numObjects,numLights,i;
  
  if (!in.good()) {
	std::cerr<<"Unable to open file \""<<fname<<"\""<<std::endl;
	abort();
  }
  
  in >> camera.fov >> camera.nearplane >> camera.farplane;
  in >> camera.eye[0] >> camera.eye[1] >> camera.eye[2];
  in >> camera.center[0] >> camera.center[1] >> camera.center[2];
  in >> camera.up[0] >> camera.up[1] >> camera.up[2];
  
  origCamera = camera;

  in >> numObjects;
  
  objects.resize(numObjects);
  
  for (i=0; i<numObjects; i++) {
	objects[i].dlid = (GLuint) i+1;
	in >> objects[i].color[0] >> objects[i].color[1] >> objects[i].color[2];
	in >> objects[i].ka >> objects[i].kd >> objects[i].ks;
	int done = 0;
	// this loop reads an arbitrary number of transformations
	// then reads the obj file name
	while (!done) {
	  in >> str;
	  if (std::strstr(str, "translate")) {
		SlVector3 translation;
		in >> translation[0] >> translation[1] >> translation[2];
		objects[i].transforms.push_back(new Translate(translation));
	  } else if (strstr(str, "rotate")) {
		double angle;
		SlVector3 axis;
		in >> angle >> axis[0] >> axis[1] >> axis[2];
		objects[i].transforms.push_back(new Rotate(angle, axis));
	  } else if (strstr(str, "scale")) {
		SlVector3 scale;
		in >> scale[0] >> scale[1] >> scale[2];
		objects[i].transforms.push_back(new Scale(scale));
	  } else {
		objects[i].readObject(str);
		done = 1;
	  }
	}
	objects[i].createDisplayList();
  }
  
  // read lights
  in >> numLights;
  lights.resize(numLights);
  
  for (i=0; i<numLights; i++) {
	in >> lights[i].ambient[0] >> lights[i].ambient[1] >> lights[i].ambient[2] >>
	  lights[i].ambient[3];
	in >> lights[i].diffuse[0] >> lights[i].diffuse[1] >> lights[i].diffuse[2] >>
	  lights[i].diffuse[3];
	in >> lights[i].specular[0] >> lights[i].specular[1] >> lights[i].specular[2] >>
	  lights[i].specular[3];
	in >> lights[i].pos[0] >> lights[i].pos[1] >> lights[i].pos[2] >> lights[i].pos[3];
	lights[i].id=GL_LIGHT0+i;
  }
}

// this routine computes the current time of day and 
// draws the clock, it assumes we're already in 2D drawing mode
void drawClock() {
  double theta;
  time_t mtime; // machine time
  tm *ltime;    // locale time
  time (&mtime);
  ltime = localtime(&mtime);
  
  double hours = ltime->tm_hour + ltime->tm_min/60.0 + ltime->tm_sec/3600.0;
  double minutes = ltime->tm_min + ltime->tm_sec/60.0;
  int seconds = ltime->tm_sec;
  
  // you still need to compute the hand directions and draw them.  
  glPushMatrix();

  glScaled(0.3,0.3,0.3);
  glRotatef(-360 * (seconds/60.0), 0,0,1.0);

  glColor3f(0.0, 0.0, 1.0);
  glBegin(GL_TRIANGLES); 
    glVertex2f(0, 1.0);
    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
  glEnd();

  glPopMatrix();

  glPushMatrix();

  glScaled(0.2,0.2,0.2);
  glRotatef(-360 * (minutes/60.0), 0,0,1.0);

  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_TRIANGLES); 
    glVertex2f(0, 1.0);
    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
  glEnd();

  glPopMatrix();

  glPushMatrix();

  glScaled(0.19,0.19,0.19);
  glRotatef(-360 * (hours/60.0), 0,0,1.0);

  glColor3f(0.0, 1.0, 0.0);
  glBegin(GL_TRIANGLES); 
    glVertex2f(0, 1.0);
    glVertex2f(-0.1, 0.0);
    glVertex2f(0.1, 0.0);
  glEnd();

  glPopMatrix();


  
}

void myReshape(int w, int h) {
  glViewport (0,0,w,h);
  viewport.w = w;
  viewport.h = h;
}

void myDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //std::cout<<"calling Display, but there's no code here"<<std::endl;
  // This is the main display function
  // You want to set up the projection using gluPerspective and gluLookAt
  // apply all the lights
  // draw all the objects
  // switch to 2d (gluOrtho2D) and draw the clock hands
  // you will need to enable various GL features (like GL_LIGHTING) here
  // this is probably the hardest function to fill in, but
  // it shouldn't be very long or complicated, it will be hard
  // to figure all the OpenGL things you have to do, it may help 
  // to look for some other OpenGL code on the web
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(camera.fov, viewport.w/viewport.h, camera.nearplane, camera.farplane);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(camera.eye[0], camera.eye[1], camera.eye[2],
	    camera.center[0], camera.center[1], camera.center[2],
	    camera.up[0], camera.up[1], camera.up[2]);

  //rotate the angle degree alone y axis
  glRotatef(360 - angle, 0, camera.eye[1], 0);

  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  if(SMOOTH_SHADING == true) {
    glShadeModel(GL_SMOOTH);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  
  //apply all lights
  for(int i = 0; i < lights.size(); i++) {
	lights[i].apply();
  }
  
  //draw all objects
  for(int i = 0; i < objects.size(); i++) {
	objects[i].drawObject();
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-0.2, 3.0, -0.2, 3.0);
  drawClock();
  glMatrixMode(GL_MODELVIEW);

  glutSwapBuffers();
}

// handle "special" (non-ascii) keyboard events
void mySpecial(int key, int x, int y) {
  double theta = 0.087266463;
  SlVector3 dir, ndir;
  switch(key) {
  case GLUT_KEY_UP:
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
      std::cout<<"shift & up key pressed";
      //Pressing the shift key while pressing up/down arrows causes the camera to tilt up.
      camera.center += theta * camera.center;
    }
    else {
      std::cout<<"up key pressed";
      // here you want to translate the camera forward
      camera.eye[0] += 0.1 * camera.center[0];
      camera.eye[2] += 0.1 * camera.center[2];
      camera.center[0] += 0.1 * camera.center[0];
      camera.center[2] += 0.1 * camera.center[2];
      // Moving forward causes the camera to return to level
      camera.center[1] = 1;
      
    }
    break;
    
  case GLUT_KEY_DOWN:
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
      std::cout<<"shift & down key pressed";
      //Pressing the shift key while pressing up/down arrows causes the camera to tilt down.
      camera.center -= theta * camera.center;
    }
    else {
      std::cout<<"down key pressed";
      // here you want to translate the camera backward
      camera.eye[0] -= 0.1 * camera.center[0];
      camera.eye[2] -= 0.1 * camera.center[2];
      camera.center[0] -= 0.1 * camera.center[0];
      camera.center[2] -= 0.1 * camera.center[2];
    }
    break;
    
  case GLUT_KEY_LEFT:
    // here you want to rotate the camera to the left or
    // if shift key is down, translate the camera left
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
      std::cout<<"shift & left key pressed";
      //translate the camera 0.1 units in the appropriate direction 
      SlVector3 n = cross(camera.center, camera.up);
      normalize(n);
      camera.eye[0] += 0.1  * n[0]; 
      camera.eye[2] += 0.1  * n[2]; 
      camera.center[0] -= 0.1 * n[0];
      camera.center[2] -= 0.1 * n[2];
    }
    else {
      std::cout<<"left key pressed";
      //change the angle 
      angle += 5;
    }
    break;
    
  case GLUT_KEY_RIGHT:
    // here you want to rotate the camera to the right or
    // if shift key is down, translate the camera right
    if (glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
      std::cout<<"shift & right key pressed";
      //translate the camera 0.1 units in the appropriate direction 
      SlVector3 n = cross(camera.center, camera.up);
      normalize(n);
      camera.eye[0] -= 0.1 * n[0];
      camera.eye[2] -= 0.1 * n[2];
      camera.center[0] += 0.1 * n[0];
      camera.center[2] += 0.1 * n[2];
    }
    else {
      std::cout<<"right key pressed";
      //change the angle
      angle -= 5;
      
    }
    break;
    
  default:
    std::cerr<<"That key is not recognized"<<std::endl;
    break;
  }
  glutPostRedisplay();
}

// handle ascii keyboard events
void myKeyboard(unsigned char key, int x, int y) {
  switch(key) {
  case 'q':
  case 'Q':
    exit(0);
    break;
  case 'o':
  case 'O':
    std::cout<<"o key pressed"<<std::endl;
    // here you want to set the current camera to the original camera
    // this is a one-liner
    camera = origCamera;
    break;
  
  case 's':
  case 'S':
    std::cout<<"s key pressed"<<std::endl;
    // here you want to toggle the smooth shading
    //toggle between flat and smooth shading
    if(SMOOTH_SHADING == true) {
      glShadeModel(GL_SMOOTH);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      SMOOTH_SHADING = false;
      FLAT_SHADING = true;
    }
    else if (SMOOTH_SHADING == false) {
      glShadeModel(GL_FLAT);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      SMOOTH_SHADING = true;
      FLAT_SHADING = false;
      
    }
    break;

  case 'w':
  case 'W':
    std::cout<<"w key pressed"<<std::endl;
    //displays objects in wireframe mode
    if(WIREFRAME == false) {
      WIREFRAME = true;
      SMOOTH_SHADING = false;
      FLAT_SHADING = false;
    }
    else {
      WIREFRAME = false;
      SMOOTH_SHADING = true;
    }
    
    break;
    
  default:
    std::cerr<<"key "<<key<<" not supported"<<std::endl;
  }
  glutPostRedisplay();
}

// timer function so that the clock is updated at least every second
void myTimerFunc(int id) {
  glutPostRedisplay();
  glutTimerFunc(1000, myTimerFunc, 0);
}

// the usual stuff, nothing exciting here
int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(400, 400);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);
  
  glutDisplayFunc(myDisplay);
  glutReshapeFunc(myReshape);
  glutKeyboardFunc(myKeyboard);
  glutSpecialFunc(mySpecial);
  glutTimerFunc(1000, myTimerFunc, 0);
  
  parseInputFile(argv[1]);
  
  glutMainLoop();
  
  return 0;
}

