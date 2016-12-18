#pragma once
#include "stdafx.h"
class CustomMath
{
private:
	static void		partition(unsigned int *indexBuffer, SoftParticle *pData, unsigned int low, unsigned int high, unsigned int pivot);
public:
	//Linear Algebra
	static float	dotVector3(D3DXVECTOR3 *first, D3DXVECTOR3 *second );

	static void		normalizeVector3( D3DXVECTOR3 *vector );
	static void		normalizeVector2( D3DXVECTOR2 *vector );

	static void		crossVector3( D3DXVECTOR3 *out, const D3DXVECTOR3 *first, const D3DXVECTOR3 *second );
	static void		crossVector4( D3DXVECTOR4 *out, const D3DXVECTOR4 *first, const D3DXVECTOR4 *second, const D3DXVECTOR4 *third);
	static void		crossMatrix3( D3DXMATRIX *out, D3DXMATRIX *first, D3DXMATRIX *second );
	static void		crossMatrix4( D3DXMATRIX *out, D3DXMATRIX *first, D3DXMATRIX *second );

	static void		createXrotMatrix( D3DXMATRIX *out, float angle );
	static void		createYrotMatrix( D3DXMATRIX *out, float angle );
	static void		createZrotMatrix( D3DXMATRIX *out, float angle );
	static void		createInverseMatrix( D3DXMATRIX *out, float *det, D3DXMATRIX *in );
	static void		createInverseWorldM( D3DXMATRIX *out, D3DXMATRIX *scale, D3DXMATRIX *rot, D3DXMATRIX *trans );
	static void		createScaleMatrix( D3DXMATRIX *out, D3DXVECTOR3 *vec );
	static void		createTransposeM( D3DXMATRIX *out, D3DXMATRIX *in );
	static void		createTranslationM( D3DXMATRIX *out, D3DXVECTOR3 *vec );
	static void		createRotationM( D3DXMATRIX *out, D3DXVECTOR3 *rot );

	static void		rotateCustomAxis( D3DXMATRIX *out, D3DXVECTOR3 *axis, float angle );
	static void		transformVector3( D3DXVECTOR3 *out, D3DXVECTOR3 *in, D3DXMATRIX *matrix );
	static void		setLookAtMatrix(D3DXMATRIX *out, D3DXVECTOR3 *position, D3DXVECTOR3 *target, D3DXVECTOR3 *up);
	static void		setPerspectiveMatrix(D3DXMATRIX *out, float volumeWidth, float volumeHeight, float zNear, float zFar);
	static void		transformCoords(D3DXVECTOR3 *out, D3DXMATRIX *m);

	static float	length(D3DXVECTOR3 *vector);
	static float	length(D3DXVECTOR3 *vector1, D3DXVECTOR3 *vector2);

	//random
	static void		setRandomSeed(unsigned int seed);
	static float	randomFloat();							//returns a float between 0.0 and 1.0
	static int		randomInteger(int min, int max);

	//index buffer sorting algorithms
	static void		quickSort(unsigned int *indexBuffer, SoftParticle *pData, UINT startIndex, UINT actualAmount);
	static void		bubbleSort(unsigned int *indexBuffer, SoftParticle *pData, UINT startIndex, UINT actualAmount, D3DXVECTOR3 *cameraPos);
};