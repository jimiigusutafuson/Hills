#include "SOParticleEffect.h"


SOParticleEffect::SOParticleEffect(D3DXVECTOR3 emitterPos, ID3D10EffectTechnique *drawTech, int maxVertices)
	:ParticleEffect(emitterPos, drawTech)
{
	firstTime							= true;

	streamOutTech						= GraphicsCore::getInstance()->g_pTechStreamOutSnow;
	streamOutputBuffer					= new VertexBuffer(VBUFFER_TYPE_STREAM_OUT, maxVertices, sizeof(SOParticle), 0, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	drawBuffer							= new VertexBuffer(VBUFFER_TYPE_STREAM_OUT, maxVertices, sizeof(SOParticle), 0, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	initBuffer							= new VertexBuffer(VBUFFER_TYPE_DYNAMIC, maxVertices, sizeof(SOParticle), 0, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	offset								= 0;
	particleAmount						= 1;
	SOParticle			*soParticleData	= NULL;
	initBuffer->actualBuffer->Map( D3D10_MAP_WRITE_DISCARD, 0, reinterpret_cast< void** >((void**)&soParticleData) );
	soParticleData[0].age					= 0;
	soParticleData[0].pos					= D3DXVECTOR3(70,400,-170);
	soParticleData[0].size				= D3DXVECTOR2(2,2);
	soParticleData[0].type				= 0; //emitter
	soParticleData[0].vel					= D3DXVECTOR3(0,.01f,0);
	initBuffer->actualBuffer->Unmap();
}
void SOParticleEffect::reset()
{
	firstTime = true;
}

void SOParticleEffect::update()
{
	GraphicsCore *core = GraphicsCore::getInstance();

	if(firstTime)
	{
		core->updateParticlesFirstTime( streamOutTech, offset, initBuffer, drawBuffer, sizeof(SOParticle) );
		firstTime = false;
	}
	else
	{
		core->updateParticles( streamOutTech, offset, particleAmount, streamOutputBuffer, drawBuffer, sizeof(SOParticle) );
		std::swap(drawBuffer, streamOutputBuffer);	//the streamout buffer is the updated one, so we switch places of the two
	}
}

void SOParticleEffect::draw()
{
	GraphicsCore::getInstance()->useVertexBuffer(drawBuffer);
	GraphicsCore::getInstance()->renderSOParticles( drawTech );
}

SOParticleEffect::~SOParticleEffect()
{
	SAFE_DELETE(streamOutputBuffer);
	SAFE_DELETE(drawBuffer);
	SAFE_DELETE(initBuffer);
}
