#include "CustomMath.h"

void CustomMath::normalizeVector3( D3DXVECTOR3 *vector )
{
	float length = sqrt( pow(vector->x,2) + pow(vector->y,2) + pow(vector->z,2) );
	vector->x /= length;
	vector->y /= length;
	vector->z /= length;
}
void CustomMath::normalizeVector2( D3DXVECTOR2 *vector )
{
	float length = sqrt( pow(vector->x,2) + pow(vector->y,2));
	vector->x /= length;
	vector->y /= length;
}
void CustomMath::crossVector3( D3DXVECTOR3 *out, const D3DXVECTOR3 *first, const D3DXVECTOR3 *second )
{
	out->x = first->y*second->z - first->z*second->y;
	out->y = first->z*second->x - first->x*second->z;
	out->z = first->x*second->y - first->y*second->x;
}

void CustomMath::crossVector4( D3DXVECTOR4 *out, const D3DXVECTOR4 *first, const D3DXVECTOR4 *second, const D3DXVECTOR4 *third)
{
	out->x = first->y	* (second->z * third->w - third->z * second->w) -
				first->z *(second->y * third->w - third->y * second->w) +
				first->w * (second->y * third->z - second->z *third->y);

	out->y = -(first->x * (second->z * third->w - third->z * second->w) -
				first->z * (second->x * third->w - third->x * second->w) +
				first->w * (second->x * third->z - third->x * second->z));

	out->z = first->x	* (second->y * third->w - third->y * second->w) -
				first->y * (second->x * third->w - third->x * second->w) +
				first->w * (second->x * third->y - third->x * second->y);

	out->w = -(first->x * (second->y * third->z - third->y * second->z) -
				first->y * (second->x * third->z - third->x *second->z) +
				first->z * (second->x * third->y - third->x * second->y));
}

void CustomMath::crossMatrix3( D3DXMATRIX *out, D3DXMATRIX *first, D3DXMATRIX *second )
{
	out->_11 = first->_11*second->_11 + first->_12*second->_21 + first->_13*second->_31;
	out->_12 = first->_11*second->_12 + first->_12*second->_22 + first->_13*second->_32;
	out->_13 = first->_11*second->_13 + first->_12*second->_23 + first->_13*second->_33;

	out->_21 = first->_21*second->_11 + first->_22*second->_21 + first->_23*second->_31;
	out->_22 = first->_21*second->_12 + first->_22*second->_22 + first->_23*second->_32;
	out->_23 = first->_21*second->_13 + first->_22*second->_23 + first->_23*second->_33;

	out->_31 = first->_31*second->_11 + first->_32*second->_21 + first->_33*second->_31;
	out->_32 = first->_31*second->_12 + first->_32*second->_22 + first->_33*second->_32;
	out->_33 = first->_31*second->_13 + first->_32*second->_23 + first->_33*second->_33;
}

void CustomMath::crossMatrix4( D3DXMATRIX *out, D3DXMATRIX *first, D3DXMATRIX *second )
{
	out->_11 = first->_11*second->_11 + first->_12*second->_21 + first->_13*second->_31 + first->_14*second->_41;
	out->_12 = first->_11*second->_12 + first->_12*second->_22 + first->_13*second->_32 + first->_14*second->_42;
	out->_13 = first->_11*second->_13 + first->_12*second->_23 + first->_13*second->_33 + first->_14*second->_43;
	out->_14 = first->_11*second->_14 + first->_12*second->_24 + first->_13*second->_34 + first->_14*second->_44;

	out->_21 = first->_21*second->_11 + first->_22*second->_21 + first->_23*second->_31 + first->_24*second->_41;
	out->_22 = first->_21*second->_12 + first->_22*second->_22 + first->_23*second->_32 + first->_24*second->_42;
	out->_23 = first->_21*second->_13 + first->_22*second->_23 + first->_23*second->_33 + first->_24*second->_43;
	out->_24 = first->_21*second->_14 + first->_22*second->_24 + first->_23*second->_34 + first->_24*second->_44;

	out->_31 = first->_31*second->_11 + first->_32*second->_21 + first->_33*second->_31 + first->_34*second->_41;
	out->_32 = first->_31*second->_12 + first->_32*second->_22 + first->_33*second->_32 + first->_34*second->_42;
	out->_33 = first->_31*second->_13 + first->_32*second->_23 + first->_33*second->_33 + first->_34*second->_43;
	out->_34 = first->_31*second->_14 + first->_32*second->_24 + first->_33*second->_34 + first->_34*second->_44;

	out->_41 = first->_41*second->_11 + first->_42*second->_21 + first->_43*second->_31 + first->_44*second->_41;
	out->_42 = first->_41*second->_12 + first->_42*second->_22 + first->_43*second->_32 + first->_44*second->_42;
	out->_43 = first->_41*second->_13 + first->_42*second->_23 + first->_43*second->_33 + first->_44*second->_43;
	out->_44 = first->_41*second->_14 + first->_42*second->_24 + first->_43*second->_34 + first->_44*second->_44;
}

float CustomMath::dotVector3(D3DXVECTOR3 *first, D3DXVECTOR3 *second )
{
	return	first->x * second->x +
			first->y * second->y +
			first->z * second->z;
}

void CustomMath::createXrotMatrix( D3DXMATRIX *out, float angle )
{
	out->_11 = 1;
	out->_12 = 0;
	out->_13 = 0;
	out->_14 = 0;
	out->_21 = 0;
	out->_22 = cos(angle);
	out->_23 = sin(angle);
	out->_24 = 0;
	out->_31 = 0;
	out->_32 = -sin(angle);
	out->_33 = cos(angle);
	out->_34 = 0;
	out->_41 = 0;
	out->_42 = 0;
	out->_43 = 0;
	out->_44 = 1;
}

void CustomMath::createYrotMatrix( D3DXMATRIX *out, float angle )
{
	out->_11 = cos(angle);
	out->_12 = 0;
	out->_13 = -sin(angle);
	out->_14 = 0;
	out->_21 = 0;
	out->_22 = 1;
	out->_23 = 0;
	out->_24 = 0;
	out->_31 = sin(angle);
	out->_32 = 0;
	out->_33 = cos(angle);
	out->_34 = 0;
	out->_41 = 0;
	out->_42 = 0;
	out->_43 = 0;
	out->_44 = 1;
}

void CustomMath::createZrotMatrix( D3DXMATRIX *out, float angle )
{
	out->_11 = cos(angle);
	out->_12 = sin(angle);
	out->_13 = 0;
	out->_14 = 0;
	out->_21 = -sin(angle);
	out->_22 = cos(angle);
	out->_23 = 0;
	out->_24 = 0;
	out->_31 = 0;
	out->_32 = 0;
	out->_33 = 1;
	out->_34 = 0;
	out->_41 = 0;
	out->_42 = 0;
	out->_43 = 0;
	out->_44 = 1;
}

void CustomMath::rotateCustomAxis( D3DXMATRIX *out, D3DXVECTOR3 *axis, float angle )
{
	//a faster approach than rotating the matrix to go along the specified axis, rotating it the angle, then turning it back.
    normalizeVector3(axis);

	out->_11 = (1.0f - cos(angle)) * axis->x * axis->x + cos(angle);
	out->_12 = (1.0f - cos(angle)) * axis->y * axis->x + sin(angle) * axis->z;
	out->_13 = (1.0f - cos(angle)) * axis->z * axis->x - sin(angle) * axis->y;
	out->_14 = 0;
	out->_21 = (1.0f - cos(angle)) * axis->x * axis->y - sin(angle) * axis->z;
	out->_22 = (1.0f - cos(angle)) * axis->y * axis->y + cos(angle);
	out->_23 = (1.0f - cos(angle)) * axis->z * axis->y + sin(angle) * axis->x;
	out->_24 = 0;
	out->_31 = (1.0f - cos(angle)) * axis->x * axis->z + sin(angle) * axis->y;
	out->_32 = (1.0f - cos(angle)) * axis->y * axis->z - sin(angle) * axis->x;
	out->_33 = (1.0f - cos(angle)) * axis->z * axis->z + cos(angle);
	out->_34 = 0;
	out->_41 = 0;
	out->_42 = 0;
	out->_43 = 0;
	out->_44 = 1;
}
void CustomMath::setLookAtMatrix( D3DXMATRIX * out, D3DXVECTOR3 *position, D3DXVECTOR3 *target, D3DXVECTOR3 *up)
{
	D3DXVECTOR3 zAxis = *target - *position;
	normalizeVector3(&zAxis);

	D3DXVECTOR3 xAxis;
	crossVector3(&xAxis, up, &zAxis);
	normalizeVector3(&xAxis);

	D3DXVECTOR3 yAxis;
	crossVector3(&yAxis, &zAxis, &xAxis);

	out->_11 = xAxis.x;
	out->_12 = yAxis.x;
	out->_13 = zAxis.x;
	out->_14 = 0;

	out->_21 = xAxis.y;
	out->_22 = yAxis.y;
	out->_23 = zAxis.y;
	out->_24 = 0;
	
	out->_31 = xAxis.z;
	out->_32 = yAxis.z;
	out->_33 = zAxis.z;
	out->_34 = 0;

	out->_41 = -dotVector3(&xAxis, position);
	out->_42 = -dotVector3(&yAxis, position);
	out->_43 = -dotVector3(&zAxis, position);
	out->_44 = 1;
}
void CustomMath::setPerspectiveMatrix(D3DXMATRIX *out, float volumeWidth, float volumeHeight, float zNear, float zFar)
{
	out->_11 = 2*zNear/volumeWidth;
	out->_12 = 0;
	out->_13 = 0;
	out->_14 = 0;

	out->_21 = 0;
	out->_22 = 2*zNear/volumeHeight;
	out->_23 = 0;
	out->_24 = 0;
	
	out->_31 = 0;
	out->_32 = 0;
	out->_33 = zFar/(zFar-zNear);
	out->_34 = 1;

	out->_41 = 0;
	out->_42 = 0;
	out->_43 = zNear*zFar/(zNear-zFar);
	out->_44 = 0;
}
void CustomMath::transformVector3( D3DXVECTOR3 *out, D3DXVECTOR3 *in, D3DXMATRIX *matrix )
{
	out->x = matrix->_11*in->x + matrix->_12*in->y + matrix->_13*in->z;
	out->y = matrix->_21*in->x + matrix->_22*in->y + matrix->_23*in->z;
	out->z = matrix->_31*in->x + matrix->_32*in->y + matrix->_33*in->z;
}

void CustomMath::transformCoords(D3DXVECTOR3 *out, D3DXMATRIX *m)
{
	float x,y,z,w;

	x = out->x*m->_11 + out->y*m->_21 + out->z*m->_31 + m->_41;
	y = out->x*m->_12 + out->y*m->_22 + out->z*m->_32 + m->_42;
	z = out->x*m->_13 + out->y*m->_23 + out->z*m->_33 + m->_43;
	w = out->x*m->_14 + out->y*m->_24 + out->z*m->_34 + m->_44;

	out->x = x/w;
	out->y = y/w;
	out->z = z/w;

}

void CustomMath::createInverseMatrix( D3DXMATRIX *out, float *det, D3DXMATRIX *in )
{
	int a, i, j;
	D3DXVECTOR4 v, vec[3];

	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			if(j != i)
			{
				a = j;
				if( j > i )
					a -= 1;
				vec[a].x = in->m[j][0];
				vec[a].y = in->m[j][1];
				vec[a].z = in->m[j][2];
				vec[a].w = in->m[j][3];
			}
		}
		crossVector4( &v, &vec[0], &vec[1], &vec[2] );
		
		out->m[0][i] = pow(-1.0f, i) * v.x / (*det);
		out->m[1][i] = pow(-1.0f, i) * v.y / (*det);
		out->m[2][i] = pow(-1.0f, i) * v.z / (*det);
		out->m[3][i] = pow(-1.0f, i) * v.w / (*det);
	}
}
void CustomMath::createInverseWorldM( D3DXMATRIX *out, D3DXMATRIX *scale, D3DXMATRIX *rot, D3DXMATRIX *trans )
{
	D3DXMATRIX invScale = (*scale);
	invScale._11 = 1/scale->_11;
	invScale._22 = 1/scale->_22;
	invScale._33 = 1/scale->_33;

	D3DXMATRIX invRot;
	createTransposeM( &invRot, rot );

	D3DXMATRIX invTrans = (*trans);
	invTrans._41 = -trans->_41;
	invTrans._42 = -trans->_42;
	invTrans._43 = -trans->_43;

	D3DXMATRIX transform;
	CustomMath::crossMatrix4(&transform, &invTrans, &invScale);
	CustomMath::crossMatrix4(out, &transform, &invRot);

	//CustomMath::->crossMatrix4(&mTransform, &mScale, &mTranslate );
	//CustomMath::->crossMatrix4(&mWorld, &mRotation, &mTransform );
	//not finished
}

void CustomMath::createScaleMatrix( D3DXMATRIX *out, D3DXVECTOR3 *vec )
{
	out->_11 = vec->x;
	out->_12 = 0;
	out->_13 = 0;
	out->_14 = 0;
	out->_21 = 0;
	out->_22 = vec->y;
	out->_23 = 0;
	out->_24 = 0;
	out->_31 = 0;
	out->_32 = 0;
	out->_33 = vec->z;
	out->_34 = 0;
	out->_41 = 0;
	out->_42 = 0;
	out->_43 = 0;
	out->_44 = 1;
}

void CustomMath::createTransposeM( D3DXMATRIX *out, D3DXMATRIX *in )
{
	out->_11 = in->_11;
	out->_12 = in->_21;
	out->_13 = in->_31;
	out->_14 = in->_41;

	out->_21 = in->_12;
	out->_22 = in->_22;
	out->_23 = in->_32;
	out->_24 = in->_42;

	out->_31 = in->_13;
	out->_32 = in->_23;
	out->_33 = in->_33;
	out->_34 = in->_43;

	out->_41 = in->_14;
	out->_42 = in->_24;
	out->_43 = in->_34;
	out->_44 = in->_44;
}

void CustomMath::createTranslationM( D3DXMATRIX *out, D3DXVECTOR3 *vec )
{
	out->_11 = 1;
	out->_12 = 0;
	out->_13 = 0;
	out->_14 = 0;
	out->_21 = 0;
	out->_22 = 1;
	out->_23 = 0;
	out->_24 = 0;
	out->_31 = 0;
	out->_32 = 0;
	out->_33 = 1;
	out->_34 = 0;
	out->_41 = vec->x;
	out->_42 = vec->y;
	out->_43 = vec->z;
	out->_44 = 1;
}

void CustomMath::createRotationM( D3DXMATRIX *out, D3DXVECTOR3 *rot )
{
	out->_11 = ( cos( rot->z )	* cos( rot->x ) )		+ ( sin( rot->z ) * sin( rot->y ) * sin( rot->x ) );
	out->_12 = sin( rot->z )	* cos( rot->y );
	out->_13 = ( cos( rot->z )	* ( -sin( rot->x ) ) )	+ (sin( rot->z ) * sin( rot->y ) * cos( rot->x ) );
	out->_14 = 0;

	out->_21 = ( -sin( rot->z )	* cos( rot->x ) )		+ ( cos( rot->z ) * sin( rot->y ) * sin( rot->x ) );
	out->_22 = cos( rot->z )	* cos( rot->y );
	out->_23 = ( sin( rot->z )	* sin( rot->x ) )		+ ( cos( rot->z ) * sin( rot->y ) * cos( rot->x ) );
	out->_24 = 0;

	out->_31 = cos( rot->y )	* sin( rot->x );
	out->_32 = ( -sin( rot->y ) );
	out->_33 = cos( rot->y )	* cos( rot->x );
	out->_34 = 0;

	out->_41 = 0;
	out->_42 = 0;
	out->_43 = 0;
	out->_44 = 1;
}

void CustomMath::setRandomSeed(unsigned int seed)
{
	srand( seed );
}

float CustomMath::randomFloat()
{
	return (float)rand()/(float)RAND_MAX;
}

int CustomMath::randomInteger(int min, int max)
{
	return (rand() % (max - min) + min);
}

void CustomMath::quickSort(unsigned int *indexBuffer, SoftParticle *pData,
							UINT startIndex, UINT actualAmount)
{

}
void CustomMath::partition(unsigned int *indexBuffer, SoftParticle *pData,
							unsigned int low, unsigned int high, unsigned int pivot)
{

}
void CustomMath::bubbleSort(unsigned int *indexBuffer, SoftParticle *pData,
							UINT startIndex, UINT actualAmount, D3DXVECTOR3 *cameraPos)
{
	unsigned int i, k, temp;
	if(actualAmount == 0)
		return;
	for(i = startIndex; i < actualAmount - 1; i++)
	{
		for(k = i; k < actualAmount - 1; k++)
		{
			if(length(&pData[indexBuffer[k]].pos, cameraPos) < length(&pData[indexBuffer[k+1]].pos, cameraPos))
			{
				temp = indexBuffer[k];
				indexBuffer[k] = indexBuffer[k+1];
				indexBuffer[k+1] = temp;
			}
		}
	}
}

float CustomMath::length(D3DXVECTOR3 *vector)
{
	return sqrt(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
}
float CustomMath::length(D3DXVECTOR3 *vector1, D3DXVECTOR3 *vector2)
{
	return length(&(*vector1 - *vector2));
}