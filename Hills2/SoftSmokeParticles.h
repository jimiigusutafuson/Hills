#pragma once
#include "particleeffect.h"
#include "CustomMath.h"

class SoftSmokeParticles :
	public ParticleEffect
{
private:
	UINT				startIndex;			//the start index of the soft particle buffer data
	UINT				maxAmount;			//maximum amount of particles
	UINT				actualAmount;		//the actual amount of particles used

	float				waitTime;			//the wait duration before another emission
	float				timeElapsed;		//time elapsed since last emission
	SoftParticle		*particles;			//pointer to the soft particle buffer data
	float				*rotationalSpeeds;	//array of rotational speed for each particle
	CustomMath			*math;
	void				resetParticle(int index);
public:
	SoftSmokeParticles( D3DXVECTOR3 emitterPos, ID3D10EffectTechnique *drawTech,
						UINT startIndex, UINT maxAmount);

	void				reset();
	void				update(float dt);
	void				updateBuffer( SoftParticle *pData ); //pData must currently be mapped to buffer
	void				draw();
	UINT				getStartIndex()		{ return startIndex; }
	UINT				getActualAmount()	{ return actualAmount; }

	~SoftSmokeParticles();
};

