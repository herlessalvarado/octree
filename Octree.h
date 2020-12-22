#ifndef OCTREE_H
#define OCTREE_H

#include <iostream>
#include <string>
#include <vector>
#include "CImg.h"
#include "Node.h"

using namespace cimg_library;
using namespace std;

class Octree {
private:
    Node* root;
	CImg<unsigned char> outputImg;
	int equation[4];

    bool uniqueColor(int xi, int xf, int yi, int yf, int zi, int zf, unsigned char img[][512][512]) {
		unsigned char pixel = img[zi][xi][yi];
		for(int k = zi; k <= zf; ++k) {
			for (int i = xi; i <= xf; ++i) {
				for (int j = yi; j <= yf; ++j) {
					if (pixel != img[k][i][j]) {
						return false;
					}
				}
			}	
		}
		return true;
	}

    void insert(int xi, int xf, int yi, int yf, int zi, int zf, unsigned char img[][512][512], Node* &n) {
        n = new Node(xi, xf, yi, yf, zi, zf);
		if (uniqueColor(xi, xf, yi, yf, zi, zf, img)) {
			n->color = img[zi][xi][yi];
            n->leaf = true;
		} else {
			insert(xi, (xf+xi)/2, yi, (yf+yi)/2, zi, (zf+zi)/2, img, n->children[0]);
			insert((xf+xi)/2+1, xf, yi, (yf+yi)/2, zi, (zf+zi)/2, img, n->children[1]);
			insert(xi, (xf+xi)/2, (yf+yi)/2+1, yf, zi, (zf+zi)/2, img, n->children[2]);
			insert((xf+xi)/2+1, xf, (yf+yi)/2+1, yf, zi, (zf+zi)/2, img, n->children[3]);
			insert(xi, (xf+xi)/2, yi, (yf+yi)/2, (zf+zi)/2+1, zf, img, n->children[4]);
			insert((xf+xi)/2+1, xf, yi, (yf+yi)/2, (zf+zi)/2+1, zf, img, n->children[5]);
			insert(xi, (xf+xi)/2, (yf+yi)/2+1, yf, (zf+zi)/2+1, zf, img, n->children[6]);
			insert((xf+xi)/2+1, xf, (yf+yi)/2+1, yf, (zf+zi)/2+1, zf, img, n->children[7]);
		}    
	}

	void equation_plane(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3){
		int a1 = x2 - x1;
		int b1 = y2 - y1;
		int c1 = z2 - z1;
		int a2 = x3 - x1;
		int b2 = y3 - y1;
		int c2 = z3 - z1;
		equation[0] = b1 * c2 - b2 * c1;
		equation[1] = a2 * c1 - a1 * c2;
		equation[2] = a1 * b2 - b1 * a2;
		equation[3] = (- equation[0] * x1 - equation[1] * y1 - equation[2] * z1);
	}

	void getCut(Node* & node) {
		if(!node) return;
		if(node->leaf) {
			for (int i = node->xi; i <= node->xf; i++) {
				for (int j = node->yi; j <= node->yf; j++) {
					for(int k = node->zi; k <= node->zf; k++) {
						if(equation[0]*i + equation[1]*j + equation[2]*k + equation[3] == 0) {
							outputImg(i,j) = node->color;
						}
					}
				}
			}
			return;
		}
		for(int i = 0; i < 8; i++) {
			getCut(node->children[i]);
		}
	}

public:
    Octree(){};

	void insert(int xi, int xf, int yi, int yf, int zi, int zf, unsigned char img[][512][512]){
		insert(xi, xf, yi, yf, zi, zf, img, root);
	}

    void getCut(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, string outputName){
		outputImg.clear();
		outputImg.resize(512,512);
		equation_plane(x1,y1,z1,x2,y2,z2,x3,y3,z3);
        getCut(root);
		string outName = "octreeOutput/" + outputName + ".jpg";
		outputImg.save(outName.c_str());
	
    }
};

#endif