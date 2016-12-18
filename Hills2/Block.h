#pragma once
#include "stdafx.h"
#include "Object3D.h"
class Block :
	public Object3D
{
private:
	unsigned int bufferOffset;
	unsigned int bufferIndexAmount;
public:
	Block( D3DXVECTOR3 position = D3DXVECTOR3(0,0,0), D3DXVECTOR3 rotation = D3DXVECTOR3(0,0,0), D3DXVECTOR3 scale = D3DXVECTOR3(1,1,1) );
	void draw(ID3D10EffectTechnique *tech);
	void feedVBuffer(UINT pDataStartIndex, Vertex *pData);
	int getBufferAmount() { return bufferIndexAmount; }
	~Block();
};