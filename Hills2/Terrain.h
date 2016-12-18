#pragma once
#include "Object3D.h"
#include <iostream>
#include <vector>
#include "HeightMapLoader.h"
#include <assert.h>
using namespace std;

class Terrain : Object3D
{
private:

	vector<D3DXVECTOR3>		groundGeometry;
	vector<D3DXVECTOR3>		normals;
	int						tileWidth;
	int						groundSizeX, groundSizeZ;
	void					generateNormals(int index1, int index2, int index3);

	UINT					bufferOffset; //start index in vertex buffer
	UINT					bufferIndexAmount; //how many vertices inside the vertex buffer

public:
	Terrain(int x = 256, int z = 256, int tileWidth = 10);
	D3DXVECTOR3				getPoint(int x, int z);
	UINT					getBufferAmount() {return bufferIndexAmount; }
	UINT					getBufferOffset() { return bufferOffset; }
	void					feedVBuffer(UINT pDataStartIndex, Vertex *pData);
	void					generateFromHeightMap(HeightMapLoader* heightMap);
	float					getHeightAtPosition(D3DXVECTOR3 pos);
	void					filter3x3();
	void					fixNormals();
	void					draw(ID3D10EffectTechnique* tech);

	~Terrain();
};

