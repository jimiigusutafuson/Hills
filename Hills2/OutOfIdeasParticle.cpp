#include "OutOfIdeasParticle.h"


OutOfIdeasParticle::OutOfIdeasParticle(D3DXVECTOR3 emitterPos, ID3D10EffectTechnique *drawTech, UINT startIndex, UINT maxAmount)
	:ParticleEffect(emitterPos, drawTech)
{
	this->startIndex		= startIndex;
	this->maxAmount			= maxAmount;
	actualAmount			= 1;
	waitTime				= 0.5f;
	timeElapsed				= .0f;
	particles				= new Particle[maxAmount];
	particles[0].pos		= emitterPos;
	particles[0].size		= D3DXVECTOR2(20,20);
}
void OutOfIdeasParticle::reset()
{
	actualAmount = 1;
}

void OutOfIdeasParticle::update(float dt)
{
	if(actualAmount < maxAmount)
	{
		timeElapsed += dt;
		if(timeElapsed >= waitTime)
		{
			timeElapsed -= waitTime;
			particles[actualAmount].pos.x	= emitterPos.x;
			particles[actualAmount].pos.y	= emitterPos.y+5;
			particles[actualAmount].pos.z	= emitterPos.z;
			particles[actualAmount].size	= D3DXVECTOR2(10,10);
			actualAmount++;
		}
	}
	for(unsigned int i = 1; i < actualAmount; i++)
	{
		particles[i].pos.y+= 0.04f;
		particles[i].size *= 0.998f;
		if(particles[i].pos.y > emitterPos.y + 80)
		{
			particles[i].pos.y	= emitterPos.y+5;
			particles[i].size	= D3DXVECTOR2(10,10);
		}
	}
}

void OutOfIdeasParticle::draw()
{
	GraphicsCore *core = GraphicsCore::getInstance();
	core->render(drawTech,startIndex,actualAmount);
}
void OutOfIdeasParticle::updateBuffer( Particle *pData )
{
	for(unsigned int i = 0; i < actualAmount; i++)
	{
		pData[startIndex + i] = particles[i];
	}
}

OutOfIdeasParticle::~OutOfIdeasParticle()
{
	SAFE_DELETE(particles);
}
