#pragma once
#include "stdafx.h"
#include "GraphicsCore.h"
class ParticleEffect
{
protected:
	D3DXVECTOR3					emitterPos;
	ID3D10EffectTechnique		*drawTech;			//used for drawing particles
	ID3D10ShaderResourceView	*texture;

public:
	ParticleEffect(D3DXVECTOR3 emitterPos, ID3D10EffectTechnique *drawTech);

	virtual void				reset();
	virtual void				setTexture(ID3D10ShaderResourceView *texture);
			void				useTexture();
	virtual void				draw();

	~ParticleEffect();
};

