#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <assert.h>
using namespace std;
class HeightMapLoader
{
private:
	string					filename;
	float					heightScale, heightOffset;
	int						width;
	int						height;
	vector<float>			mHeightMap;
public:
	HeightMapLoader(void);
	void loadRAW(int width, int height,
				const string& filename,
				float heightScale,
				float heightOffset);
	//returns the total length of mHeightMap
	int						getMapWidth()	{ return width; }
	int						getMapHeight()	{ return height; }
	//returns the specified point
	float getHeightAt(int x, int y);
	//returns the whole heightMap
	vector<float>			getHeightMap()	{ return mHeightMap; }
	~HeightMapLoader(void);
};

