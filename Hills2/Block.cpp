#include "Block.h"

//the -2 values are for modelIDs and such, to be able to write new object resently added in the edit mode directly to
//the level file, this is not implemented/used yet. 
Block::Block(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale)
	:Object3D(position, rotation, scale)
{
}
void Block::feedVBuffer(UINT pDataStartIndex, Vertex *pData)
{
	bufferOffset = pDataStartIndex;
	bufferIndexAmount = 0;
	//front of the cube
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,-5.0f,-5.0f), D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DXVECTOR2(0.499f,0.332f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,5.0f,-5.0f), D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DXVECTOR2(0.0f,0.0f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,5.0f,-5.0f), D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DXVECTOR2(0.499f,0.0f));

	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,-5.0f,-5.0f), D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DXVECTOR2(0.499f,0.332f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,-5.0f,-5.0f), D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DXVECTOR2(0.0f,0.332f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,5.0f,-5.0f), D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DXVECTOR2(0.0f,0.0f));

	//right side of the cube
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,-5.0f,-5.0f), D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DXVECTOR2(1.0f,0.666f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,5.0f,5.0f), D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DXVECTOR2(0.501f,0.334f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,5.0f,-5.0f), D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DXVECTOR2(1.0f,0.334f));

	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,-5.0f,-5.0f), D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DXVECTOR2(1.0f,0.666f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,-5.0f,5.0f), D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DXVECTOR2(0.501f,0.666f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,5.0f,5.0f), D3DXVECTOR3(-1.0f,0.0f,0.0f),D3DXVECTOR2(0.501f,0.334f));

	//back of the cube
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,-5.0f,5.0f), D3DXVECTOR3(0.0f,0.0f,1.0f),D3DXVECTOR2(0.499f,0.666f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,5.0f,5.0f), D3DXVECTOR3(0.0f,0.0f,1.0f),D3DXVECTOR2(0.0f,0.334f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,5.0f,5.0f), D3DXVECTOR3(0.0f,0.0f,1.0f),D3DXVECTOR2(0.499f,0.334f));

	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,-5.0f,5.0f), D3DXVECTOR3(0.0f,0.0f,1.0f),D3DXVECTOR2(0.499f,0.666f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,-5.0f,5.0f), D3DXVECTOR3(0.0f,0.0f,1.0f),D3DXVECTOR2(0.0f,0.666f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,5.0f,5.0f), D3DXVECTOR3(0.0f,0.0f,1.0f),D3DXVECTOR2(0.0f,0.334f));

	//left side of the cube
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,-5.0f,5.0f), D3DXVECTOR3(1.0f,0.0f,0.0f),D3DXVECTOR2(1.0f,0.332f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,5.0f,-5.0f), D3DXVECTOR3(1.0f,0.0f,0.0f),D3DXVECTOR2(0.501f,0.0f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,5.0f,5.0f), D3DXVECTOR3(1.0f,0.0f,0.0f),D3DXVECTOR2(1.0f,0.0f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,-5.0f,5.0f), D3DXVECTOR3(1.0f,0.0f,0.0f),D3DXVECTOR2(1.0f,0.332f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,-5.0f,-5.0f), D3DXVECTOR3(1.0f,0.0f,0.0f),D3DXVECTOR2(0.501f,0.332f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,5.0f,-5.0f), D3DXVECTOR3(1.0f,0.0f,0.0f),D3DXVECTOR2(0.501f,0.0f));

	//top of the cube
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,-5.0f,-5.0f), D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DXVECTOR2(0.501f,0.668f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,-5.0f,5.0f), D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DXVECTOR2(1.0f,1.0f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,-5.0f,5.0f), D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DXVECTOR2(0.501f,1.0f));

	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,-5.0f,-5.0f), D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DXVECTOR2(0.501f,0.668f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,-5.0f,-5.0f), D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DXVECTOR2(1.0f,0.668f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,-5.0f,5.0f), D3DXVECTOR3(0.0f,-1.0f,0.0f),D3DXVECTOR2(1.0f,1.0f));

	//bottom of the cube
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,5.0f,5.0f), D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,0.668f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,5.0f,-5.0f), D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.499f,1.0f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,5.0f,-5.0f), D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,1.0f));

	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(-5.0f,5.0f,5.0f), D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.0f,0.668f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,5.0f,5.0f), D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.499f,0.668f));
	pData[ pDataStartIndex + bufferIndexAmount++ ] = Vertex( D3DXVECTOR3(5.0f,5.0f,-5.0f), D3DXVECTOR3(0.0f,1.0f,0.0f),D3DXVECTOR2(0.499f,1.0f));
	
}

void Block::draw(ID3D10EffectTechnique *tech)
{
	GraphicsCore *core = GraphicsCore::getInstance();
	useShaderVars();
	core->render(tech, bufferOffset, bufferIndexAmount);
}

Block::~Block()
{
}