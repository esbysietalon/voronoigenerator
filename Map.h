#pragma once
#include <random>
#include "time.h"
#include <vector>

struct intpair {
	int x, y;
	intpair(int a, int b) {
		x = a;
		y = b;
	}
	intpair() {
		x = 0;
		y = 0;
	}
};
struct vcell {
	std::vector<int> ilist;
	int color;
	intpair seed;
};
class Map {
public:
	Map(int w, int h, int v);
	~Map();
	int* generateMap();
	int* generateMapJoined();
	
	int getW();
	int getH();
private:
	intpair* generatePoints(int numPoints);
	vcell* generateVoronoi();
	vcell* layerMap(vcell* rawmap);

	int width, height;
	int vseed;
	int vjoin;
	int* map;
	std::mt19937 dev{ time(NULL) };
};