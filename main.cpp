#define cimg_display  1
#define cimg_use_png  1
#define cimg_use_jpeg 1

#include "CImg.h"
#include "Octree.h"
#include <string>

using namespace cimg_library;
using namespace std;

void Binarizar(CImg<float> & img, int umbral, int z_plane, unsigned char cube[][512][512]){
    for(int i=0; i<img.width(); i++){
        for(int j=0; j<img.height(); j++){
            int r = img(i,j,0);
            int g = img(i,j,1);
            int b = img(i,j,2);
            if ( (r+g+b)/3 > umbral)
                cube[z_plane][i][j] = (char) 0;
            else
                cube[z_plane][i][j] = (char) 255;
        }
    }
};

void getCutFromCube(int xi, int xf, int yi, int yf, int z, unsigned char img[][512][512]){
    CImg<unsigned char> outputImg;
    outputImg.resize(xf-xi,yf-yi);
    for(int i = xi; i < xf; i++){
        for(int j = yi; j < yf; j++){
            outputImg(i-xi,j-yi) = img[z][i][j];
        }
    }
    outputImg.display();
}

int main() {
    string path = "data/paciente 1/1/Paciente1CC-27-10-1988- CT from 18-01-2011 S0 I";
    static unsigned char cube[40][512][512];
    for(int i = 0; i < 40; i++) {
        string tomography = path + to_string(i) + ".BMP";
        CImg<float> R(tomography.c_str());
        Binarizar(R,128,i,cube);
        R.display();
    }
    for(int z = 0; z < 40; z++) {
        getCutFromCube(200,400,200,400,z,cube);
    }
	Octree octree;
    octree.insert(0, 511, 0, 511, 0, 39, cube);
	octree.getCut(0,0,0,511,0,0,0,511,0);
    return 0;
}
