#pragma once
#include "particleeffect.h"
class OutOfIdeasParticle :
	public ParticleEffect
{
private:
	UINT				startIndex;			//the start index of the soft particle buffer data
	UINT				maxAmount;			//maximum amount of particles
	UINT				actualAmount;		//the actual amount of particles used
	float				waitTime;			//the wait duration before another emission
	float				timeElapsed;		//time elapsed since last emission
	Particle			*particles;			//particle buffer data for this particle group only (will be mapped into the global particle buffer data)
public:
	OutOfIdeasParticle(D3DXVECTOR3 emitterPos, ID3D10EffectTechnique *drawTech, UINT startIndex, UINT maxAmount);
	void				reset();
	void				update(float dt);
	void				draw();
	void				updateBuffer( Particle *pData );
	~OutOfIdeasParticle();
};

