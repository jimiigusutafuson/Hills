#include "Object3D.h"
#include "CustomMath.h"


Object3D::Object3D()
{
	init(D3DXVECTOR3(0,0,0), D3DXVECTOR3(0,0,0), D3DXVECTOR3(1,1,1));
}
Object3D::Object3D(D3DXVECTOR3 position)
{
	init(position, D3DXVECTOR3(0,0,0), D3DXVECTOR3(1,1,1));
}
Object3D::Object3D(D3DXVECTOR3 position, D3DXVECTOR3 rotation)
{
	init(position, rotation, D3DXVECTOR3(1,1,1));
}
Object3D::Object3D(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale)
{
	init(position, rotation, scale);
}
void Object3D::init(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale)
{
	this->scale = scale;
	this->position = position;
	this->rotation = rotation;

	ka = D3DXVECTOR3(.2f,.2f,.2f);
	kd = D3DXVECTOR3(.7f,.7f,.7f);
	ks = D3DXVECTOR3( 0.1f, 0.1f, 0.1f);

	updateMatrices();
}


void Object3D::setPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}
void Object3D::setRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}
void Object3D::setScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}

void Object3D::draw(ID3D10EffectTechnique* tech)
{
}

void Object3D::useShaderVars()
{
	GraphicsCore *core = GraphicsCore::getInstance();
	//core->useKaKdKs(ka,kd,ks);

	core->setShaderVector3("ka", &ka);
	core->setShaderVector3("kd", &kd);
	core->setShaderVector3("ks", &ks);
	core->useWorldMatrix(mWorld);

}
void Object3D::updateMatrices()
{
	//D3DXMatrixScaling(&mScale, scale.x, scale.y, scale.z);
	CustomMath::createScaleMatrix(&mScale, &scale);

	//CustomMath::normalizeVector3(&rotation);

	//CustomMath::CreateRotationM( &mRotation, &rotation );
	//D3DXMatrixRotationYawPitchRoll( &mRotation, rotation.x, rotation.y, rotation.z );
	CustomMath::createRotationM( &mRotation, &rotation );

	//D3DXMatrixTranslation( &mTranslate, position.x, position.y, position.z );
	CustomMath::createTranslationM( &mTranslate, &position );


	//D3DXMatrixMultiply( &mWorld, &mRotation, &mTranslate );
	D3DXMATRIX mTransform;
	CustomMath::crossMatrix4(&mTransform, &mScale, &mTranslate );
	CustomMath::crossMatrix4(&mWorld, &mRotation, &mTransform );

	CustomMath::createInverseWorldM(&mWorldInv, &mScale, &mRotation, &mTranslate);
}
Object3D::~Object3D(void)
{
}
