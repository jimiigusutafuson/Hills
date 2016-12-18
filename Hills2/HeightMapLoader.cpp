#include "HeightMapLoader.h"


HeightMapLoader::HeightMapLoader(void)
{
}
void HeightMapLoader::loadRAW(int width, int height,
							const string& filename,
							float heightScale,
							float heightOffset)
{
	this->filename = filename;
	this->heightScale = heightScale;
	this->heightOffset = heightOffset;
	this->width = width;
	this->height = height;

	vector<unsigned char> vertexHeights(width*height);

	ifstream fin;
	fin.open(filename.c_str(), ios_base::binary);
	if(!fin)
		throw exception("error opening file");
	fin.read((char *)&vertexHeights[0], (streamsize)vertexHeights.size());
	fin.close();
	mHeightMap = vector<float>();
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			int k = i*height + j;
			mHeightMap.push_back(vertexHeights[k] * heightScale + heightOffset);
		}
	}
}
float HeightMapLoader::getHeightAt(int x, int y)
{
	assert(x < width && y < height);
	return mHeightMap.at(y * width + x);
}
HeightMapLoader::~HeightMapLoader(void)
{
}
