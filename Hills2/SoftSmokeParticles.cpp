#include "SoftSmokeParticles.h"


SoftSmokeParticles::SoftSmokeParticles( D3DXVECTOR3 emitterPos, ID3D10EffectTechnique *drawTech, UINT startIndex, UINT maxAmount)
	:ParticleEffect(emitterPos, drawTech)
{

	this->startIndex		= startIndex;
	this->maxAmount			= maxAmount;
	actualAmount			= 0;
	waitTime				= 1.0f;
	timeElapsed				= .0f;
	rotationalSpeeds		= new float[maxAmount];
	particles				= new SoftParticle[maxAmount];
}
void SoftSmokeParticles::reset()
{
	actualAmount			= 1;
	rotationalSpeeds[0]		= CustomMath::randomFloat();
}

void SoftSmokeParticles::update(float dt)
{
	if(actualAmount < maxAmount)
	{
		timeElapsed += dt;
		if(timeElapsed >= waitTime)
		{
			timeElapsed -= waitTime;
			resetParticle(actualAmount);
			actualAmount++;
		}
	}
	for(unsigned int i = 0; i < actualAmount; i++)
	{
		if(particles[i].alpha <= 0)
			resetParticle(i);
		
		if(particles[i].size.x < 9.16)
			particles[i].alpha	+= .005f;
		else
			particles[i].alpha	-= .0005f;

		particles[i].pos.y		+= .002f;
		particles[i].rotation	+=	rotationalSpeeds[i];
		particles[i].size		+= D3DXVECTOR2(.02f, .02f);
	}
}
void SoftSmokeParticles::resetParticle(int index)
{
	rotationalSpeeds[index]		= (CustomMath::randomFloat() - .5f) *.005f;	//ranges from -0.5 to 0.5
	particles[index].pos.x		= emitterPos.x-10 + CustomMath::randomInteger(0,20);
	particles[index].pos.y		= emitterPos.y;
	particles[index].pos.z		= emitterPos.z-10 + CustomMath::randomInteger(0,20);
	particles[index].alpha		= .01f;
	particles[index].rotation	= 0;
	particles[index].size		= D3DXVECTOR2(5.2f, 5.2f);
}
void SoftSmokeParticles::updateBuffer( SoftParticle *pData )
{
	for(unsigned int i = 0; i < actualAmount; i++)
	{
		pData[startIndex + i] = particles[i];
	}
}

void SoftSmokeParticles::draw()
{
	GraphicsCore *core = GraphicsCore::getInstance();
	//core->render(drawTech,startIndex,actualAmount);
	core->renderIndexed(drawTech,startIndex,0,actualAmount);
}

SoftSmokeParticles::~SoftSmokeParticles()
{
	SAFE_DELETE(particles);
	SAFE_DELETE(rotationalSpeeds);
}
