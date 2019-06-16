#include "stdafx.h"
#include "Map.h"

Map::Map(int w, int h, int v) {
	width = w;
	height = h;
	vseed = v;
}

Map::~Map()
{
}

intpair* Map::generatePoints(int numPoints) {
	std::uniform_int_distribution<int> xRNG{ 0, width };
	std::uniform_int_distribution<int> yRNG{ 0, height };

	intpair* output = new intpair[numPoints];

	for (int i = 0; i < numPoints; i++) {
		int randX = xRNG(dev);
		int randY = yRNG(dev);

		output[i] = intpair(randX, randY);
	}
	return output;
}

vcell* Map::generateVoronoi() {
	intpair* initialPoints = generatePoints(vseed);
	
	vcell* output = new vcell[vseed];
	std::uniform_real_distribution<double> cRNG{ 0, 1 };

	for (int i = 0; i < vseed; i++) {
		vcell cell;
		cell.color = 0xFF * cRNG(dev) + 0x0100 * (0xFF * cRNG(dev)) + 0x010000 * (0xFF * cRNG(dev));
		output[i] = cell;
	}

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			float dist = INFINITY;
			int icell = -1;
			for (int i = 0; i < vseed; i++) {
				int vx = initialPoints[i].x;
				int vy = initialPoints[i].y;

				int vDist = (vy - y) * (vy - y) + (vx - x) * (vx - x);
				if (vDist < dist) {
					icell = i;
					dist = vDist;
				}
			}
			output[icell].ilist.push_back(x + y * width);
		}
	}
	return output;
}

int* Map::generateMap() {
	vcell* vmap = generateVoronoi();
	int* output = new int[width * height];
	for (int i = 0; i < vseed; i++) {
		for (int j = 0; j < vmap[i].ilist.size(); j++) {
			output[vmap[i].ilist.at(j)] = vmap[i].color;
		}
	}
	return output;
}

int Map::getW() {
	return width;
}
int Map::getH() {
	return height;
}

int* Map::getColorMap() {
	int* cmap = new int[width * height];
	for (int i = 0; i < width * height; i++)
		cmap[i] = 0x00ff00;
	return cmap;
}