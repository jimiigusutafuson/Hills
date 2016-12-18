#pragma once
#ifndef _STDAFX__H
#define _STDAFX__H

#include <stdint.h>

#include <windows.h>
#include <D3D10.h>
#include <D3DX10.h>

#include <string>
#include <vector>

#define PI (3.14159265358979323846f)
#define _SCREENWIDTH (1024)
#define _SCREENHEIGTH (768)
#define PLAYERHEIGHT (15)

//memory leak detection
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

struct Vertex
{
	D3DXVECTOR3			pos;
	D3DXVECTOR2			texCoord;
	D3DXVECTOR3			normal;
	Vertex( D3DXVECTOR3 inpos = D3DXVECTOR3(0,0,0), D3DXVECTOR3 innormal = D3DXVECTOR3(0,0,0), D3DXVECTOR2 intexCoord = D3DXVECTOR2(0,0) )
	{
		pos			= inpos;
		normal		= innormal;
		texCoord	= intexCoord;
	}
};

struct Particle
{
	D3DXVECTOR3			pos;
	D3DXVECTOR2			size;
};

struct SOParticle
{
	D3DXVECTOR3			pos;
	D3DXVECTOR2			size;
	D3DXVECTOR3			vel;
	float				age;		
	UINT				type;
};

struct SoftParticle
{
	D3DXVECTOR3			pos;
	D3DXVECTOR2			size;
	float				rotation;
	float				alpha;
};

#define SAFE_RELEASE(x) if( x ) { (x)->Release(); (x) = NULL; }
#define SAFE_DELETE(x) if( x ) { delete(x); (x) = NULL; }

typedef 
enum vBufferType
{
	VBUFFER_TYPE_DEFAULT	= 0,
	VBUFFER_TYPE_DYNAMIC	= 1,
	VBUFFER_TYPE_STREAM_OUT	= 2,
	VBUFFER_TYPE_IMMUTABLE	= 3
};

#endif