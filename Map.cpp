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
		cell.seed = initialPoints[i];
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

vcell* Map::layerMap(vcell* rawmap, int vjoin) {
	intpair* initialPoints = generatePoints(vjoin);
	vcell* output = new vcell[vjoin];
	std::uniform_real_distribution<double> cRNG{ 0, 1 };
	for (int i = 0; i < vjoin; i++) {
		vcell cell;
		cell.seed = initialPoints[i];
		cell.color = 0xFF * cRNG(dev) + 0x0100 * (0xFF * cRNG(dev)) + 0x010000 * (0xFF * cRNG(dev));
		output[i] = cell;
	}
	for (int j = 0; j < vseed; j++) {
		float dist = INFINITY;
		int icell = -1;
		int ox = rawmap[j].seed.x;
		int oy = rawmap[j].seed.y;
		for (int i = 0; i < vjoin; i++) {
			int vx = initialPoints[i].x;
			int vy = initialPoints[i].y;

			float vDist = (oy - vy) * (oy - vy) + (ox - vx) * (ox - vx);
			if (vDist < dist) {
				dist = vDist;
				icell = i;
			}
		}
		output[icell].ilist.insert(output[icell].ilist.end(), rawmap[j].ilist.begin(), rawmap[j].ilist.end());
	}

	return output;
}

intpair averagePoints(intpair* group, int len) {
	intpair output = intpair(0,0);
	for (int i = 0; i < len; i++) {
		output.x += group[i].x;
		output.y += group[i].y;
	}
	output.x = ((double)output.x) / len;
	output.y = ((double)output.y) / len;
	return output;
}

vcell* Map::layerMapComplex(vcell * rawmap, int join, int seedgroupsize)
{
	intpair** initialPointGroups = new intpair*[join];
	for (int i = 0; i < join; i++) {
		initialPointGroups[i] = generatePoints(seedgroupsize);
	}
	vcell* output = new vcell[join];
	std::uniform_real_distribution<double> cRNG{ 0, 1 };
	for (int i = 0; i < join; i++) {
		vcell cell;

		cell.seed = averagePoints(initialPointGroups[i], join);
		cell.color = 0xFF * cRNG(dev) + 0x0100 * (0xFF * cRNG(dev)) + 0x010000 * (0xFF * cRNG(dev));
		output[i] = cell;
	}
	for (int j = 0; j < vseed; j++) {
		float dist = INFINITY;
		int icell = -1;
		int ox = rawmap[j].seed.x;
		int oy = rawmap[j].seed.y;
		for (int i = 0; i < join; i++) {
			for (int k = 0; k < seedgroupsize; k++) {
				int vx = initialPointGroups[i][k].x;
				int vy = initialPointGroups[i][k].y;
				float vDist = (oy - vy) * (oy - vy) + (ox - vx) * (ox - vx);
				if (vDist < dist) {
					dist = vDist;
					icell = i;
				}
			}
		}
		output[icell].ilist.insert(output[icell].ilist.end(), rawmap[j].ilist.begin(), rawmap[j].ilist.end());
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

int * Map::generateMapJoined(int numcells, int seedgroupsize)
{
	vcell* vmap;
	if (seedgroupsize < 2) {
		vmap = layerMap(generateVoronoi(), numcells);
	}
	else {
		vmap = layerMapComplex(generateVoronoi(), numcells, seedgroupsize);
	}
	int* output = new int[width * height];
	for (int i = 0; i < numcells; i++) {
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
