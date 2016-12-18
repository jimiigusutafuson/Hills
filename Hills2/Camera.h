#pragma once
#include "stdafx.h"
#include "GraphicsCore.h"
#include "CustomMath.h"
class Camera
{
private:
	CustomMath				*math;
	D3DXVECTOR3				position;
	D3DXVECTOR3				target;
	D3DXVECTOR3				up;
	D3DXMATRIX				rotx;
	D3DXMATRIX				rotz;
	D3DXMATRIX				*mView;
	D3DXMATRIX				*mViewInv;
	D3DXMATRIX				*mProj;
	D3DXMATRIX				rotationMatrix;
	D3DXVECTOR3				strafeVec;

	float					pitchRotation;

	void					updateProjView();
public:
	Camera(float posX, float posY, float posZ);

	

	D3DXMATRIX				*getViewMatrix() { return mView; }
	D3DXMATRIX				*getProjectionMatrix() { return mProj; }
	D3DXVECTOR3				getPosition() { return position; }
	D3DXVECTOR3				getDirection();
	void					setHeight(float y);
	void					move(D3DXVECTOR3 movement);
	void					fly(D3DXVECTOR3 movement);
	void					rotate(float x, float y, float z);

	~Camera();
};

