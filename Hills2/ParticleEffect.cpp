#include "ParticleEffect.h"


ParticleEffect::ParticleEffect(D3DXVECTOR3 emitterPos, ID3D10EffectTechnique *drawTech)
{
	this->emitterPos = emitterPos;
	this->drawTech = drawTech;
}

void ParticleEffect::reset()
{

}

void ParticleEffect::draw()
{
}

void ParticleEffect::setTexture(ID3D10ShaderResourceView *texture)
{
	this->texture = texture;
}

void ParticleEffect::useTexture()
{
	GraphicsCore::getInstance()->useResource(texture, "pTexture");
}

ParticleEffect::~ParticleEffect()
{
}
