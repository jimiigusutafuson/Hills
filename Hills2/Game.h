#pragma once
#include "GraphicsCore.h"
#include "Camera.h"
#include "Terrain.h"
#include "HeightMapLoader.h"
#include "SOParticleEffect.h"
#include "SoftSmokeParticles.h"
#include "OutOfIdeasParticle.h"
#include "Input.h"
#include "Block.h"
class Game
{
private:
	VertexBuffer			*vBufferStatic;			//vertex buffer for static vertices
	VertexBuffer			*vBufferSoftParticle;	//vertex buffer for soft particles
	VertexBuffer			*vBufferParticles;		//vertex buffer for normal particles
	
	unsigned int			*indicesSoftParticle;	//index array for soft particles
	unsigned int			*newmap;				//new array for soft particles
	ID3D10Buffer			*iBufferSoftParticle;	//index buffer for soft particle vertex buffer

	Vertex					*pData;
	Particle				*particleData;
	SoftParticle			*sParticleData;

	SoftSmokeParticles		*softSmoke;
	SOParticleEffect		*snow;
	OutOfIdeasParticle		*outOfIdeasParticle;

	Camera					*camera;
	Terrain					*terrain;
	Block					*block;
	HeightMapLoader			*hMapLoader;
	D3DXVECTOR3				sun;
	Input					*input;

	float					timeStep;
	float					gameTime;
	int						fps;
	float					timeSinceFPSUpdate;
	bool					flying;
public:
	Game();
	void update(float dt);
	void draw();
	~Game();
};

