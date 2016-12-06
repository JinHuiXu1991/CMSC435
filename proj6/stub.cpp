/*****************************************
** File:    stub.cpp
** Project: CMSC 435 Assignment 6, Fall 2015
** Authoer: Jin Hui Xu
** Date:    12/05/2015
** E-mail:  ac39537@umbc.edu
**
**    This file contains main driver and the source code of stub class for assignment 6.
** This program contains the implementations of methods and member functions
** for stub class.
**
*****************************************/

#define cimg_display 0
#include "CImg.h"
#include "slVector.H"
#include <vector>
#include<cstdlib>
#include<cstring>
#include <iostream>
using namespace std;
using namespace cimg_library;


//global variables
int width, height;
vector <int> horizontalSeam;
vector <int> verticalSeam;


/*use the dual gradient energy function: The energy of pixel (x, y) is ¦¤x2(x, y) + ¦¤y2(x, y), 
where the square of the x-gradient ¦¤x2(x, y) = Rx(x, y)2 + Gx(x, y)2 + Bx(x, y)2, and where 
the central differences Rx(x, y), Gx(x, y), and Bx(x, y) are the absolute value in differences of 
red, green, and blue components between pixel (x + 1, y) and pixel (x - 1, y), and
pixel (x, y + 1) and pixel (x, y - 1).
*/
void energy_function(SlVector3 *image, double *energy) {
        SlVector3 Xenergy, Yenergy;

	for (int i=0; i<width; i++) {
		for (int j=0; j<height; j++) {
			//check leftmost edge pixel
			if(i == 0 && j != 0 && j != height - 1) {
				Xenergy[0] = image[(i+1)*height+j][0] - image[width*height+j][0];
				Xenergy[1] = image[(i+1)*height+j][1] - image[width*height+j][1];
				Xenergy[2] = image[(i+1)*height+j][2] - image[width*height+j][2];
	
				Yenergy[0] = image[i*height+(j+1)][0] - image[i*height+(j-1)][0];
				Yenergy[1] = image[i*height+(j+1)][1] - image[i*height+(j-1)][1];
				Yenergy[2] = image[i*height+(j+1)][2] - image[i*height+(j-1)][2];
			}
			//check top edge pixel
			else if(i != 0 && i != width -1 && j == 0) {
				Xenergy[0] = image[(i+1)*height+j][0] - image[(i-1)*height+j][0];
				Xenergy[1] = image[(i+1)*height+j][1] - image[(i-1)*height+j][1];
				Xenergy[2] = image[(i+1)*height+j][2] - image[(i-1)*height+j][2];
	
				Yenergy[0] = image[i*height+(j+1)][0] - image[i*height+height][0];
				Yenergy[1] = image[i*height+(j+1)][1] - image[i*height+height][1];
				Yenergy[2] = image[i*height+(j+1)][2] - image[i*height+height][2];
			}
			//check rightmost edge pixel
			else if(i == width-1 && j != height-1 && j != 0) {
				Xenergy[0] = image[0*height+j][0] - image[(i-1)*height+j][0];
				Xenergy[1] = image[0*height+j][1] - image[(i-1)*height+j][1];
				Xenergy[2] = image[0*height+j][2] - image[(i-1)*height+j][2];

				Yenergy[0] = image[i*height+(j+1)][0] - image[i*height+(j-1)][0];
				Yenergy[1] = image[i*height+(j+1)][1] - image[i*height+(j-1)][1];
				Yenergy[2] = image[i*height+(j+1)][2] - image[i*height+(j-1)][2];
			}
			//check bottom edge pixel
			else if(i != width-1 && i != 0 && j == height-1) {
				Xenergy[0] = image[(i+1)*height+j][0] - image[(i-1)*height+j][0];
				Xenergy[1] = image[(i+1)*height+j][1] - image[(i-1)*height+j][1];
				Xenergy[2] = image[(i+1)*height+j][2] - image[(i-1)*height+j][2];

				Yenergy[0] = image[i*height+0][0] - image[i*height+(j-1)][0];
				Yenergy[1] = image[i*height+0][1] - image[i*height+(j-1)][1];
				Yenergy[2] = image[i*height+0][2] - image[i*height+(j-1)][2];
			}
			//check top left corner pixel
			else if(i == 0 && j == 0) {
				Xenergy[0] = image[(i+1)*height+j][0] - image[width*height+j][0];
				Xenergy[1] = image[(i+1)*height+j][1] - image[width*height+j][1];
				Xenergy[2] = image[(i+1)*height+j][2] - image[width*height+j][2];

				Yenergy[0] = image[i*height+(j+1)][0] - image[i*height+height][0];
				Yenergy[1] = image[i*height+(j+1)][1] - image[i*height+height][1];
				Yenergy[2] = image[i*height+(j+1)][2] - image[i*height+height][2];
			}
			//check top right corner pixel
			else if(i == width - 1 && j == 0) {
				Xenergy[0] = image[0*height+j][0] - image[(i-1)*height+j][0];
				Xenergy[1] = image[0*height+j][1] - image[(i-1)*height+j][1];
				Xenergy[2] = image[0*height+j][2] - image[(i-1)*height+j][2];

				Yenergy[0] = image[i*height+(j+1)][0] - image[i*height+height][0];
				Yenergy[1] = image[i*height+(j+1)][1] - image[i*height+height][1];
				Yenergy[2] = image[i*height+(j+1)][2] - image[i*height+height][2];
			}
			//check bottom left corner pixel
			else if(i == 0 && j == height - 1) {
				Xenergy[0] = image[(i+1)*height+j][0] - image[width*height+j][0];
				Xenergy[1] = image[(i+1)*height+j][1] - image[width*height+j][1];
				Xenergy[2] = image[(i+1)*height+j][2] - image[width*height+j][2];

				Yenergy[0] = image[i*height+0][0] - image[i*height+(j-1)][0];
				Yenergy[1] = image[i*height+0][1] - image[i*height+(j-1)][1];
				Yenergy[2] = image[i*height+0][2] - image[i*height+(j-1)][2];
			}
			//check bottom right corner pixel
			else if(i == width-1 && j == height-1) {
				Xenergy[0] = image[0*height+j][0] - image[(i-1)*height+j][0];
				Xenergy[1] = image[0*height+j][1] - image[(i-1)*height+j][1];
				Xenergy[2] = image[0*height+j][2] - image[(i-1)*height+j][2];

				Yenergy[0] = image[i*height+0][0] - image[i*height+(j-1)][0];
				Yenergy[1] = image[i*height+0][1] - image[i*height+(j-1)][1];
				Yenergy[2] = image[i*height+0][2] - image[i*height+(j-1)][2];
			}
			//non bounder pixel
			else {
				Xenergy[0] = image[(i+1)*height+j][0] - image[(i-1)*height+j][0];
				Xenergy[1] = image[(i+1)*height+j][1] - image[(i-1)*height+j][1];
				Xenergy[2] = image[(i+1)*height+j][2] - image[(i-1)*height+j][2];
	
				Yenergy[0] = image[i*height+(j+1)][0] - image[i*height+(j-1)][0];
				Yenergy[1] = image[i*height+(j+1)][1] - image[i*height+(j-1)][1];
				Yenergy[2] = image[i*height+(j+1)][2] - image[i*height+(j-1)][2];
			}

			//calculate the energy for pixel
			energy[i*height+j] = Xenergy[0] * Xenergy[0] + Xenergy[1] * Xenergy[1] + Xenergy[2] * Xenergy[2] +
				             Yenergy[0] * Yenergy[0] + Yenergy[1] * Yenergy[1] + Yenergy[2] * Yenergy[2];

		}
	}
}


//find the minimum path of horizontal seam on the image
void findHorizontalSeam(double *energy){

	double min = 10000000.0;
	int index = 0;

	//find the minimum starting point on the leftmost edge
	for (int j=0; j<height; j++) {
		if (energy[0*height+j] < min) {	
			min = energy[0*height+j];
			index = 0*height+j;
		}
	}
	horizontalSeam.push_back(index);

	//loop over the width of image and find the minimum for each width
	for (int i=1; i<width; i++) {
		double min = 100000000000.0;
		int temp = 0;

		//check top edge pixel
		if(index % height == 0) {
			if (energy[index+1*height] < min) {	
				min = energy[index+1*height];
				temp = index+1*height;
			}

			if (energy[index+1*height+1] < min) {	
				min = energy[index+1*height+1];
				temp = index+1*height+1;
			}
		}
		//check bottom edge pixel
		else if(index % height == height - 1) {
			if (energy[index+1*height] < min) {	
				min = energy[index+1*height];
				temp = index+1*height;
			}
			
			if (energy[index+1*height-1] < min) {	
				min = energy[index+1*height-1];
				temp = index+1*height-1;
			}
		}
		else {
			if (energy[index+1*height-1] < min) {	
				min = energy[index+1*height-1];
				temp = index+1*height-1;
			}

			if (energy[index+1*height] < min) {	
				min = energy[index+1*height];
				temp = index+1*height;
			}

			if (energy[index+1*height+1] < min) {	
				min = energy[index+1*height+1];
				temp = index+1*height+1;
			}
		}
		
		index = temp;
		horizontalSeam.push_back(index);
	}
}


//find the minimum path of vertical seam on the image
void findVerticalSeam(double *energy) {

	double min = 100000000000.0;
	int index = 0;

	//find the minimum starting point on the top edge
	for (int i=0; i<width; i++) {
		if (energy[i*height+0] < min) {	
			min = energy[i*height+0];
			index = i*height+0;
		}
	}
	verticalSeam.push_back(index);

	//loop over the height of image and find the minimum for each height
	for (int j=1; j<height; j++) {
		double min = 10000000.0;
		int temp = 0;

		//check leftmost edge pixel
		if(index  < height) {
			if (energy[index+1] < min) {
				min = energy[index+1] ;
				temp = index+1;
			}

			if (energy[index+1*height+1] < min) {	
				min = energy[index+1*height+1];
				temp = index+1*height+1;
			}

		}
		//check rightmost edge pixel
		else if(width*height - index <= height) {
			if (energy[index+1] < min) {
				min = energy[index+1] ;
				temp = index+1;
			}

			if (energy[index-1*height+1] < min) {
				min = energy[index-1*height+1] ;
				temp = index-1*height+1;
			}
		}
		else {
			if (energy[index+1] < min) {
				min = energy[index+1] ;
				temp = index+1;
			}

			if (energy[index-1*height+1] < min) {
				min = energy[index-1*height+1] ;
				temp = index-1*height+1;
			}

			if (energy[index+1*height+1] < min) {	
				min = energy[index+1*height+1];
				temp = index+1*height+1;
			}
		}
		
		index = temp;
		verticalSeam.push_back(index);
	}
}


//remove the minimum path of horizontal seam on the image and reform the array to proper format
void removeHorizontalSeam(SlVector3 *image) {
	//loop over image array to delete the horizontal seam path
	for(int i=0; i<width; i++) {
		for (int j = horizontalSeam[i]; j<(i+1)*height-1; j++) {
			image[i*height+(j%height)][0] = image[i*height+(j%height)+1][0];
			image[i*height+(j%height)][1] = image[i*height+(j%height)+1][1];
			image[i*height+(j%height)][2] = image[i*height+(j%height)+1][2];
		}
		image[i*height+height-1][0] = 0;
		image[i*height+height-1][1] = 0;
		image[i*height+height-1][2] = 0;
	}

	//loop over image array to delete any 0 content on the array
	//reform the image array to proper format for next seam carving
	for(int j = 0; j<width; j++) {
		for (int i=(j+1)*height-(j+1); i<width*height; i++) {
			image[i][0] = image[i+1][0];
			image[i][1] = image[i+1][1];
			image[i][2] = image[i+1][2];
  		}
	}

	//decrement height after each seam carving
	height --;

	//clear the horizontal seam path vector for next seam carving
	horizontalSeam.clear();

}


//remove the minimum path of vertical seam on the image and reform the array to proper format
void removeVerticalSeam(SlVector3 *image) {
	//loop over image array to delete the vertical seam path
	for (int j=0; j< height; j++) {
		for (int i = verticalSeam[j]/height; i<width-1; i++) {
			image[i*height+j][0] = image[(i+1)*height+j][0];
			image[i*height+j][1] = image[(i+1)*height+j][1];
			image[i*height+j][2] = image[(i+1)*height+j][2];
		}

		image[(width-1)*height+j][0] = 0;
		image[(width-1)*height+j][1] = 0;
		image[(width-1)*height+j][2] = 0;
	}

	//decrement width after each seam carving
	width --;

	//clear the vertical seam path vector for next seam carving
	verticalSeam.clear();

}


int main(int argc, char *argv[]) {
	CImg<double> input(argv[1]);
	CImg<double> lab = input.RGBtoLab();
	SlVector3 *image = new SlVector3[input.width()*input.height()];
	int newWidth, newHeight, wLoopCount, hLoopCount;

	width = input.width();
	height = input.height();

   
	for (int i=0; i<input.width(); i++) {
		for (int j=0; j<input.height(); j++) {
			image[i*input.height()+j][0] = lab(i, j, 0);
	  		image[i*input.height()+j][1] = lab(i, j, 1);
	  		image[i*input.height()+j][2] = lab(i, j, 2);
		}
	}

	newWidth = atoi(argv[3]);
	newHeight = atoi(argv[4]);

	//calculate the different of current dimension and desired dimension
	wLoopCount = width - newWidth;
	hLoopCount = height - newHeight;

	//do horizontal carving until image has shrunk to the desired dimension:
	while(wLoopCount > 0) {
		double *energy = new double[width*height];
		energy_function(image, energy);
		findVerticalSeam(energy);
		removeVerticalSeam(image);
		wLoopCount --;
	}

	//do vertical carving until image has shrunk to the desired dimension:
	while(hLoopCount > 0) {
		double *energy = new double[width*height];
		energy_function(image, energy);
		findHorizontalSeam(energy);
		removeHorizontalSeam(image);
		hLoopCount --;
	}

  	CImg<double> output(atoi(argv[3]), atoi(argv[4]), input.depth(), input.spectrum(), 0);
 	for (int i=0; i<output.width(); i++) {
		for (int j=0; j<output.height(); j++) {
	  		output(i, j, 0) = image[i*output.height()+j][0];
	 		output(i, j, 1) = image[i*output.height()+j][1];
	  		output(i, j, 2) = image[i*output.height()+j][2];
		}
  	}
  	CImg<double> rgb = output.LabtoRGB();
  	if (strstr(argv[2], "png"))
		rgb.save_png(argv[2]);
 	else if (strstr(argv[2], "jpg"))
		rgb.save_jpeg(argv[2]);

  	delete [] image;
  	return 0;
}
