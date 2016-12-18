#include "Camera.h"


Camera::Camera(float posX, float posY, float posZ)
{

	position		=	D3DXVECTOR3(posX,posY,posZ);
	target			=	D3DXVECTOR3(posX,posY,posZ+1);
	up				=	D3DXVECTOR3(0,1,0);
	rotationMatrix	=	D3DXMATRIX(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	pitchRotation	=	0;
	mView			=	new D3DXMATRIX();
	mViewInv		=	new D3DXMATRIX();
	mProj			=	new D3DXMATRIX();
	CustomMath::setPerspectiveMatrix(mProj, (float)D3DX_PI * 0.6f, (float)(_SCREENWIDTH / _SCREENHEIGTH), 1.0f, 10000.0f);
	//D3DXMatrixPerspectiveLH(mProj, (float)D3DX_PI * 0.6f, (float)(_SCREENWIDTH / _SCREENHEIGTH), 1.0f, 10000.0f);
	updateProjView();
}
void Camera::updateProjView()
{
	//CustomMath::setLookAtMatrix(mView, &position, &target, &up);
	D3DXMatrixLookAtLH(mView, &position, &target, &up);
	float temp = D3DXMatrixDeterminant(mView);
	D3DXMatrixInverse(mViewInv, &temp, mView);

	GraphicsCore *core = GraphicsCore::getInstance();
	core->useViewMatrix(*mView);
	core->useProjectionMatrix(*mProj);
	core->setShaderVector3("cameraPos", &position);
	core->setShaderMatrix("mViewInv", mViewInv);

	D3DXVECTOR3 test = D3DXVECTOR3(1,1,1);
	CustomMath::transformVector3(&test,&test,mView);
	CustomMath::transformVector3(&test,&test,mViewInv);
}
void Camera::move(D3DXVECTOR3 movement)
{
	D3DXVECTOR3 vDirection;

	//D3DXVec3Normalize(&vDirection,
	//				&(target - position)); //create direction vector
	vDirection = target - position;
	vDirection.y = 0;
	CustomMath::normalizeVector3(&vDirection);
	CustomMath::crossVector3(&strafeVec,&vDirection, &up);
	CustomMath::normalizeVector3(&strafeVec);

	//horizontal movement
	position += strafeVec * movement.x;
	//vertical movement
	position.y += movement.y;
	//forward movement
    position += vDirection * movement.z;

	//target follows
	target += strafeVec * movement.x;
	target.y += movement.y;
    target += vDirection * movement.z;

    updateProjView();
}
void Camera::fly(D3DXVECTOR3 movement)
{
	D3DXVECTOR3 vDirection;
	

	//D3DXVec3Normalize(&vDirection,
	//				&(target - position)); //create direction vector
	vDirection = target - position;
	CustomMath::normalizeVector3(&vDirection);
	CustomMath::crossVector3(&strafeVec,&vDirection, &up);
	CustomMath::normalizeVector3(&strafeVec);
	//horizontal movement
	position += strafeVec * movement.x;
	//vertical movement
	position.y += movement.y;
	//forward movement
    position += vDirection * movement.z;

	target = position + vDirection;

    updateProjView(); 
}
void Camera::rotate(float x, float y, float z)
{
	if(pitchRotation - y/180 > 270)
		y = max(pitchRotation,270) - min(pitchRotation,270);
	else if(pitchRotation - y/180 < -270)
		y = min(pitchRotation,-270) - max(pitchRotation,-270);
	pitchRotation -= y;
	D3DXVECTOR3 vDirection,vRotAxis;

	//D3DXVec3Normalize( &vDirection, &(target - position) ); //create direction vector
	vDirection = target - position;
	CustomMath::normalizeVector3(&vDirection);

	//D3DXVec3Cross(&vRotAxis,&vDirection,&up); //strafe vector
	CustomMath::crossVector3(&vRotAxis,&vDirection,&up);
	//D3DXVec3Normalize(&vRotAxis,&vRotAxis);
	CustomMath::normalizeVector3(&vRotAxis);

	//create rotation matrices
	CustomMath::rotateCustomAxis(&rotationMatrix,
                       &vRotAxis,
                       y / -180);
	/*D3DXMatrixRotationAxis(&rotationMatrix,
                       &vRotAxis,
                       y / -180);*/
	
	CustomMath::createYrotMatrix(&rotx,x / 180);
	CustomMath::createZrotMatrix(&rotz,z / -180);  //troublemaker! flips camera sometimes (resolved)
	//D3DXMatrixRotationY(&rotx,x / 180);
	//D3DXMatrixRotationZ(&rotz,z / -180);

	rotationMatrix = rotationMatrix * rotz * rotx;

	//rotate direction
	CustomMath::transformCoords(&vDirection, &(rotationMatrix));
	//D3DXVec3TransformCoord(&vDirection,&vDirection,&(rotationMatrix));
	//rotate up vector
	CustomMath::transformCoords(&up, &(rotationMatrix));
	//D3DXVec3TransformCoord(&up,&up,&(rotationMatrix));
	//translate up vector
	target = vDirection + position;

	updateProjView();
}

void Camera::setHeight(float y)
{
	target.y += (y - position.y);
	position.y = y;
}

D3DXVECTOR3 Camera::getDirection()
{
	D3DXVECTOR3 vDirection = target - position;
	CustomMath::normalizeVector3(&vDirection);
	return vDirection;
}

Camera::~Camera()
{
	SAFE_DELETE(mView);
	SAFE_DELETE(mProj);
}
