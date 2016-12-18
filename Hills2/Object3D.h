#pragma once
#include "stdafx.h"
#include "GraphicsCore.h"
class Object3D
{
private:
	D3DXVECTOR3 scale;
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;

	D3DXMATRIX	mRotation,
				mScale,
				mTranslate,
				mWorld,
				mWorldInv;

protected:

	D3DXVECTOR3		ka, kd, ks;

	void		setRotation(float x, float y, float z);
	void		setPosition(float x, float y, float z);
	void		setScale(float x, float y, float z);

	void		useShaderVars();

public:
	Object3D();
	Object3D(D3DXVECTOR3 position);
	Object3D(D3DXVECTOR3 position, D3DXVECTOR3 rotation);
	Object3D(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale);
	void init(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale);

	virtual void draw(ID3D10EffectTechnique* tech);
	void		updateMatrices();
	D3DXMATRIX		getWorld()	{ return mWorld; }
	D3DXMATRIX		getWorldInv() { return mWorldInv; }
	D3DXVECTOR3		getPosition() { return position; }

	~Object3D();
};

