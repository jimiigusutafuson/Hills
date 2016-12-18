#pragma once
#include "particleeffect.h"
class SOParticleEffect :
	public ParticleEffect
{
private:
	VertexBuffer			*streamOutputBuffer;	//this points to the buffer receiving stream out data.			(swaps with drawBuffer)
	VertexBuffer			*drawBuffer;			//we use this buffer to draw. This is always the updated one.	(swaps with streamOutBuffer)
	VertexBuffer			*initBuffer;			//initializing buffer is filled by the CPU.
	ID3D10EffectTechnique	*streamOutTech;			//used for moving and generating particles.
	UINT					particleAmount;
	UINT					offset;
	bool					firstTime;

public:
	SOParticleEffect(D3DXVECTOR3 emitterPos, ID3D10EffectTechnique *drawTech,
						int maxVertices);

	void					reset();
	void					update();
	void					draw();

	~SOParticleEffect();
};

