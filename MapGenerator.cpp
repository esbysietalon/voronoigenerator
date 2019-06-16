// MapGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Skeleton.h"
#include "Map.h"
#include <iostream>

int main(int argc, char** argv)
{
	Skeleton s(800, 600);
	Map map(800, 600, 10000);

	int maptexture = s.createTexture(map.generateMapJoined(4, 4), map.getW(), map.getH());
	s.createSprite(maptexture, 0, 0);
	
	std::function<void()>* farr = new std::function<void()>[1];
	farr[0] = std::bind(&Skeleton::render, &s);

	while (s.listen() > 0) {
		s.capFrames(60, farr, 1);
	}
    return 0;
}

