#include "VertexBuffer.h"
#include "GraphicsCore.h"


VertexBuffer::VertexBuffer(vBufferType type, int vertexAmount, size_t vertexStructSize, const D3D10_SUBRESOURCE_DATA *pData, D3D_PRIMITIVE_TOPOLOGY topology, UINT offset)
{
	this->vertexAmount = vertexAmount;
	this->vertexStructSize = vertexStructSize;
	this->offset = offset;
	this->topology = topology;

	switch(type)
	{
		case VBUFFER_TYPE_DYNAMIC:
		{
			initDynamic();
			break;
		}
		case VBUFFER_TYPE_STREAM_OUT:
		{
			initStreamOut();
			break;
		}
		case VBUFFER_TYPE_IMMUTABLE:
		{
			initImmutable();
			break;
		}
		default:
		{
			initDefault();
		}
	}

	actualBuffer = NULL;
	
	GraphicsCore::getInstance()->createVertexBuffer(this, pData);
}

void VertexBuffer::initImmutable()
{
	bufferDesc.Usage			= D3D10_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth		= vertexStructSize * vertexAmount;
	bufferDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags	= 0;
	bufferDesc.MiscFlags		= 0;
}

void VertexBuffer::initDynamic()
{
	bufferDesc.Usage			= D3D10_USAGE_DYNAMIC;
	bufferDesc.ByteWidth		= vertexStructSize * vertexAmount;
	bufferDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags		= 0;
}
void VertexBuffer::initStreamOut()
{
	bufferDesc.Usage			= D3D10_USAGE_DEFAULT;
	bufferDesc.ByteWidth		= vertexStructSize * vertexAmount;
	bufferDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER | D3D10_BIND_STREAM_OUTPUT;
	bufferDesc.CPUAccessFlags	= 0;
	bufferDesc.MiscFlags		= 0;
}
void VertexBuffer::initDefault()
{
	bufferDesc.Usage			= D3D10_USAGE_DEFAULT;
	bufferDesc.ByteWidth		= vertexStructSize * vertexAmount;
	bufferDesc.BindFlags		= D3D10_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags	= 0;
	bufferDesc.MiscFlags		= 0;
}
VertexBuffer::~VertexBuffer()
{
	SAFE_RELEASE(actualBuffer);
}
