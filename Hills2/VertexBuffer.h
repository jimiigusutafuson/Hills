#pragma once
#include "stdafx.h"
class VertexBuffer
{
	friend class GraphicsCore;
private:
	D3D10_BUFFER_DESC		bufferDesc;
	int						vertexAmount;
	size_t					vertexStructSize; //stride
	UINT					offset;
	D3D_PRIMITIVE_TOPOLOGY	topology;

	void					initImmutable();
	void					initDynamic();
	void					initStreamOut();
	void					initDefault();

public:
	ID3D10Buffer			*actualBuffer;
	int						getVertexAmount()		{ return vertexAmount; }
	size_t					getVertexStructSize()	{ return vertexStructSize; }
	UINT					getOffset()				{ return offset; }
	D3D_PRIMITIVE_TOPOLOGY	getTopology()			{ return topology; }

	VertexBuffer(vBufferType type, int vertexAmount, size_t vertexStructSize,
					const D3D10_SUBRESOURCE_DATA *pData = 0,
					D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
					UINT offset = 0);
	~VertexBuffer();
};

