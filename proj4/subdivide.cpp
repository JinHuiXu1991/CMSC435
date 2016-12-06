/*****************************************
** File:    subdivide.cpp
** Project: CMSC 435 Assignment 4, Fall 2015
** Authoer: Jin Hui Xu
** Date:    11/07/2015
** E-mail:  ac39537@umbc.edu
**
**    This file contains main driver and the source code of subdivide class for assignment 4.
** This program contains the implementations of methods and member functions
** for subdivide class.
**
*****************************************/

#include "subdivide.H"
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

//define key which contains two index of vertices for unordered map
typedef pair<unsigned int, unsigned int> points;

//half edge structures
struct Vectex;
struct Face;

struct HalfEdge
{
	HalfEdge* pair;		
	HalfEdge* previous;
	HalfEdge* next;
	Vectex* vectex;
	Face* face;
	bool flag;
};

struct Vectex
{
	vector<HalfEdge*> halfedge;
	unsigned int index;
	bool flag;
	SlVector3 pos;
	SlVector3 newpos;
};

struct Face
{
	vector<HalfEdge*> halfedge;
};

//hash function for unordered map
struct hashFunct {
    size_t operator()(const  points &points ) const
    {
        return hash<unsigned int>()(points.first) ^ hash<unsigned int>()(points.second);
    }
};

//global variables
vector<HalfEdge*> hes;
vector<Face> faces;
vector<Vectex> vectexs;


void readObject(char *fname) {
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
  in.close();

}


void initialize_HE() {
	std::unordered_map<points, HalfEdge*, hashFunct > Edges;

	//initialize faces, each face points to three half edges
	for(unsigned int i = 0; i < triangles.size(); i++) {
		Face f;
		for(int j = 0; j <= 2; j++) {
			unsigned int a, b;
			a = triangles[i][j];
			if(j != 2) 
				b = triangles[i][j+1];
			else
				b = triangles[i][0];
			Edges[points(a, b)] = new HalfEdge();
			f.halfedge.push_back(Edges[points(a, b)]);
			
		}
		
		faces.push_back(f);
	}

	//initialize vectexs
	for(unsigned int i = 0; i < vertices.size(); i++) {
		Vectex v;
		v.index = i;
		v.pos = vertices[i];
		vectexs.push_back(v);
	}

	//initialize half edge of vectexs, each vectex points to all outgoing half edges
	for(unsigned int i = 0; i < triangles.size(); i++) {
		
		for(int j = 0; j <= 2; j++) {
			unsigned int a, b;
			a = triangles[i][j];
			if(j != 2) 
				b = triangles[i][j+1];
			else
				b = triangles[i][0];
			
			for(unsigned int k = 0; k < vectexs.size(); k++) {
				if(vertices[a] == vectexs[k].pos) {
					vectexs[k].halfedge.push_back(Edges[points(a, b)]);
				}

			}
		}
	}

	//initialize half edges, loop through all faces
	for(unsigned int i = 0; i < faces.size(); i++) {
		
		//set each half edge points to one face
		for(unsigned int j = 0; j < faces[i].halfedge.size(); j++) {
			faces[i].halfedge[j]->face = &faces[i];
		}

		//loop through all half edges for each face
		for(unsigned int j = 0; j < faces[i].halfedge.size(); j++) {	
				
			unsigned int a, b;
			a = triangles[i][j];
			if(j != 2) 
				b = triangles[i][j+1];
			else
				b = triangles[i][0];

			//set certain half edge points to next half edge
			if(j != faces[i].halfedge.size() - 1) 
				faces[i].halfedge[j]->next = faces[i].halfedge[j+1];
			
			else 
				faces[i].halfedge[j]->next = faces[i].halfedge[0];

			//set certain half edge points to previous half edge			
			if(j > 0) 
				faces[i].halfedge[j]->previous = faces[i].halfedge[j-1];
			else if(j == 0)
				faces[i].halfedge[j]->previous = faces[i].halfedge[faces[i].halfedge.size() - 1];
			
			//set certain half edge points to pair half edge 
			//and its pair half edge points to certain half edge
			if(Edges.find(points(b, a)) != Edges.end()) {
				Edges[points(a, b)]->pair = Edges[points(b, a)];
				Edges[points(b, a)]->pair = Edges[points(a, b)];
			}

		
			//set each half edge points to one source vectex
			for(unsigned int k = 0; k < vectexs.size(); k++) {
				if(vertices[a] == vectexs[k].pos){
					Edges[points(a, b)]->vectex = &vectexs[k];
				}
			}

		}
		
	}

	//store all halfedge pointers into a vector structure
	for(auto i = Edges.begin(); i != Edges.end(); ++i) {
		hes.push_back(i->second);
	}

}


void subdiv(){
	/*

	//mark old vertice as true
	for(unsigned int i = 0; i < vectexs.size(); i++) {
		vectexs[i].flag = true;
	}

	//mark every half edge as not slipted
	for(unsigned int i = 0; i < hes.size(); i++) {
		hes[i]->flag = false;
	}
	
	//update old vertices locations
	for(unsigned int i = 0; i < vectexs.size(); i++) {
		SlVector3 sum;
		if(vectexs[i].flag == true){
			if(vectexs[i].halfedge.size() > 3) {
				//old vertice new position formula for n > 3
				//(5/8) * original_position + (3/8) * average_of_1-ring_neighbors
				for(unsigned int k = 0; k<vectexs[i].halfedge.size(); k++) {
					sum += vectexs[i].halfedge[k]->next->vectex->pos;
				}
				vectexs[i].newpos = (5/8.0) * vectexs[i].pos + (3/8.0) * (sum / vectexs[i].halfedge.size());
			}
			else {
				//old vertice new position formula for n = 3
				//(1-(3*(3/16))) * original_position + (3/16) * average_of_1-ring_neighbors 
				sum = vectexs[i].halfedge[0]->next->vectex->pos + 
				      vectexs[i].halfedge[1]->next->vectex->pos + 
				      vectexs[i].halfedge[2]->next->vectex->pos;
				vectexs[i].newpos = (7/16.0) * vectexs[i].pos + (3/16.0) * (sum / 3);

			}
			
		}
	}

	//loop through each half edgs to split
	for(unsigned int i = 0; i < hes.size(); i++) {
		//check if current half edge is divided already
		if(hes[i]->flag == false) {
			Vectex v;
			HalfEdge newedge;
			HalfEdge newedge2;
			v.index = vectexs.size();
			v.flag = false;


			//calculate the location of new vertices
			//check if it is boundary 
			if(hes[i]->pair != NULL){
				v.pos = (3/8.0) * hes[i]->vectex->pos + 
			         	(3/8.0) * hes[i]->next->vectex->pos + 
			      		(1/8.0) * hes[i]->previous->vectex->pos + 
			      		(1/8.0) * hes[i]->pair->previous->vectex->pos;
			}
			else{
				v.pos = (1/2.0) * hes[i]->vectex->pos + (1/2.0) * hes[i]->next->vectex->pos;
			}
			
			//start split edge
			//dividing each edge in four half edges and updating pointers in the halfedge data structure
			newedge.vectex = hes[i]->vectex;
			newedge.previous = hes[i]->previous;
			hes[i]->previous = &newedge;
			newedge.next = hes[i];
			
			newedge2.vectex = &v;
			newedge2.next = hes[i]->pair->next;
			hes[i]->pair->next = &newedge2;
			newedge2.previous = hes[i]->pair;

			hes[i]->pair = hes[i]->pair;
			hes[i]->pair->pair = hes[i];
			newedge.pair = &newedge2;
			newedge2.pair = &newedge;

			//update the flag for each visited half edge
			hes[i]->flag = true;
			hes[i]->pair->flag = true;
			newedge.flag = true;
			newedge2.flag = true;

			//update half edges in each vectex 
			v.halfedge.push_back(hes[i]);		
			v.halfedge.push_back(&newedge2);
			hes[i]->vectex = &v;


			//update half edges in each face
			//each face should become a hexagon with six half edges after subdivision
			for(unsigned int j = 0; j < hes[i]->face->halfedge.size(); j++) {
				if(hes[i] == hes[i]->face->halfedge[j])
					hes[i]->face->halfedge[j] = &newedge;
			}
			hes[i]->face->halfedge.push_back(hes[i]);
			hes[i]->pair->face->halfedge.push_back(&newedge2);
			
			//store the added vectex in the vector of vectexs
			vectexs.push_back(v);

			//store the added two half edges in the vector of half edges
			hes.push_back(&newedge);
			hes.push_back(&newedge2);
			
		}
	}

	//loop through each faces
	for(unsigned int i = 0; i < faces.size(); i++) {
		//if current face has more than 3 edges which mean it is not triangle
		if(faces[i].halfedge.size() > 3) {
			//loop through six edge to find the new added edge and its vertice
			for(unsigned int k = 0; k < 6; k++) {
				if(faces[i].halfedge[k]->vectex->halfedge.size() < 3) {
					Face f;
					HalfEdge newedge, newedge2;
					
					//start split face
					//set both new edges as already split
					newedge.flag = true;
					newedge2.flag = true;

					//updare current face information
					for(unsigned int j = 0; j < faces[i].halfedge.size(); j++) {
						if(faces[i].halfedge[k] == faces[i].halfedge[j])
							faces[i].halfedge.erase(faces[i].halfedge.begin()+j);
						if(faces[i].halfedge[k]->next == faces[i].halfedge[j])
							faces[i].halfedge.erase(faces[i].halfedge.begin()+j);
					}	
					
					//update all necessary pointers
					newedge.vectex = faces[i].halfedge[k]->vectex->halfedge[0]->next->next->vectex;
					newedge2.vectex = faces[i].halfedge[k]->vectex->halfedge[0]->vectex;

					faces[i].halfedge[k]->vectex->halfedge[0]->previous->next = &newedge2;
					newedge2.previous = faces[i].halfedge[k]->vectex->halfedge[0]->previous;

					faces[i].halfedge[k]->vectex->halfedge[0]->next->next->previous = &newedge2;
					newedge2.next = faces[i].halfedge[k]->vectex->halfedge[0]->next->next;
					
					faces[i].halfedge[k]->vectex->halfedge[0]->previous = &newedge;
					newedge.next = faces[i].halfedge[k]->vectex->halfedge[0];

					faces[i].halfedge[k]->vectex->halfedge[0]->next->next = &newedge;
					newedge.previous = faces[i].halfedge[k]->vectex->halfedge[0]->next;

					newedge.pair = &newedge2;
					newedge2.pair = &newedge;

					//add the new face into vector of faces
					f.halfedge.push_back(faces[i].halfedge[k]);
					f.halfedge.push_back(faces[i].halfedge[k]->next);
					f.halfedge.push_back(&newedge);
					faces.push_back(f);

					
				}
			}	
		}
	}
	*/
}


void output(char *fname) {
	std::ofstream out(fname, std::ios::out);

  
	if (!out.good()) {
		std::cerr<<"Unable to open file \""<<fname<<"\""<<std::endl;
		abort();
	}
  
	if(out.good()) {
		//write vertices 
		for(unsigned int i = 0; i < vectexs.size(); i++) {
			//check if the vectice is old or added one
			//if vectice is added one, print its position, if is old one, print its new position
			if(vectexs[i].flag == false)
				out << "v " << vectexs[i].pos[0] << " " << vectexs[i].pos[1] << " " << vectexs[i].pos[2] << endl;
			else
				out << "v " << vectexs[i].newpos[0] << " " << vectexs[i].newpos[1] << " " << vectexs[i].newpos[2] << endl;

		}
		//write faces
		//loop through each face to get and print its halfedges' original vectice's index
		for(unsigned int i = 0; i < faces.size(); i++) {
			out << "f " << faces[i].halfedge[0]->vectex->index << " " << faces[i].halfedge[1]->vectex->index << " " << faces[i].halfedge[2]->vectex->index << endl;
		}
	}
	out.close();
}


int main(int argc, char *argv[]) {
	int x;
	//check arguments inputs
	if (argc != 4) {
		std::cout<<"usage: subdivide input.obj output.obj number_of_subdivision_steps"<<std::endl;
		exit(0);
	
	}

	//read input file and initialize halfedge structure
	readObject(argv[1]);
	initialize_HE();
	
	//start loop subdivision
	x = strtol(argv[3], NULL, 10);
	for(int i = 0; i < x; i++){
		subdiv();
	}
	//print output file
	output(argv[2]);
  
	return 0;
}
