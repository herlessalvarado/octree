#include "Octree.h"

int main() {
	Octree octree;
	octree.getCut(0,0,0,255,0,0,0,255,0);
    octree.getCut(255,255,0,255,255,0,0,0,0);
    return 0;
}
