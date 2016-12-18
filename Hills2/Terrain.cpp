#include "Terrain.h"
#include "CustomMath.h"

Terrain::Terrain(int x, int z, int tileWidth)
{
	groundSizeX		= x;
	groundSizeZ		= z;
	this->tileWidth	= tileWidth;
	groundGeometry	= vector<D3DXVECTOR3>();
	normals			= vector<D3DXVECTOR3>();
	for(int i = 0; i < z; i++)
		for(int k = 0; k < x; k++)
		{
			groundGeometry.push_back( D3DXVECTOR3(tileWidth*k*1.0f,.0f,-tileWidth*i*1.0f) );
			normals.push_back( D3DXVECTOR3(0,0,0) );
		}
	//creates the vertex buffer for all the static vertex data
	setPosition(-500,-20,500);
	//setPosition(0,0,0);
	setRotation(0,0,0);
	setScale(1,1,1);
	updateMatrices();

}
void Terrain::generateFromHeightMap(HeightMapLoader* heightMap)
{
	assert(heightMap != NULL);
	for(int y = 0; y < heightMap->getMapHeight(); y++)
		for(int x = 0; x < heightMap->getMapWidth(); x++)
			groundGeometry.at(y*groundSizeX +x).y = heightMap->getHeightAt(x,y);
	filter3x3();
	fixNormals();
}

float Terrain::getHeightAtPosition(D3DXVECTOR3 pos)
{
	//transform position to this world
	CustomMath::transformCoords(&pos, &getWorldInv() );

	// don't check further if outside map
	if(pos.x < groundGeometry.at( 0 ).x || pos.x > groundGeometry.at( groundSizeX - 1 ).x ||
		pos.z < groundGeometry.at(groundSizeZ*groundSizeX - 1 ).z  || pos.z > groundGeometry.at(0).z)
	{
		return 0;
	}

	//plane equation ax + by + cz + d = 0
	//y = -(ax + cz + d) / b;

	int vPosX = (int)(pos.x / tileWidth);
	int vPosZ = (int)(-pos.z / tileWidth);


	D3DXVECTOR3 p1, p2, p3; //the triangle points

if(pos.x - vPosX * tileWidth < (-pos.z) - vPosZ * tileWidth)
	{
		// 1--2
		// | /
		// |/
		// 3
		/*p1 = groundGeometry.at( vPosZ * groundSizeX			+ vPosX );
		p2 = groundGeometry.at( vPosZ * groundSizeX			+ vPosX + 1 );
		p3 = groundGeometry.at( ( vPosZ + 1 ) * groundSizeX + vPosX );*/

		// 1
		// |\
		// | \
		// 2--3
		p1 = groundGeometry.at( vPosZ * groundSizeX			+ vPosX );
		p2 = groundGeometry.at( ( vPosZ + 1 ) * groundSizeX + vPosX );
		p3 = groundGeometry.at( ( vPosZ + 1 ) * groundSizeX + vPosX + 1);
	}
	else
	{
		//    3
		//   /|
		//  / |
		// 2--1
		/*p1 = groundGeometry.at( ( vPosZ + 1 ) * groundSizeX + vPosX + 1);
		p2 = groundGeometry.at( ( vPosZ + 1 ) * groundSizeX + vPosX );
		p3 = groundGeometry.at( vPosZ * groundSizeX			+ vPosX + 1 );*/


		// 1--3
		//  \ |
		//   \|
		//    2
		p1 = groundGeometry.at( vPosZ * groundSizeX			+ vPosX );
		p2 = groundGeometry.at( ( vPosZ + 1 ) * groundSizeX + vPosX + 1);
		p3 = groundGeometry.at( vPosZ * groundSizeX			+ vPosX + 1 );
	}

	D3DXVECTOR3 e1 = p3-p1;
	D3DXVECTOR3 e2 = p2-p1;

	//cross product
	D3DXVECTOR3 normal;
	normal.x = e1.y*e2.z - e1.z*e2.y;
	normal.y = e1.z*e2.x - e1.x*e2.z;
	normal.z = e1.x*e2.y - e1.y*e2.x;

	//NORMALIZE THE VECTOR
	float magnitude = sqrt((normal.x * normal.x) +(normal.y * normal.y) + (normal.z * normal.z));
	normal.x /= magnitude;
	normal.z /= magnitude;
	normal.y /= magnitude;

	//ax + by + cz + d = 0

	//rearange
	//  d = - (ax + by + cz )
	float D = -((normal.x * p1.x) + (normal.y * p1.y) + (normal.z * p1.z));

	// y =  - (ax +  cz  + d  ) / b

	pos.y = - ((normal.x * pos.x) + (normal.z * pos.z) + D ) / normal.y;

	CustomMath::transformCoords(&pos, &getWorld());

	return pos.y;
}

D3DXVECTOR3 Terrain::getPoint(int x, int z)
{
	assert(x < groundSizeX && z < groundSizeZ);
	return groundGeometry.at(z*groundSizeX + x);
}
void Terrain::feedVBuffer(UINT pDataStartIndex, Vertex *pData)
{
	bufferOffset = pDataStartIndex;
	bufferIndexAmount = 0;
	for(int z = 0; z < groundSizeZ - 1; z++)
	{
		for(int x = 0; x < groundSizeX - 1; x++)
		{
			////clockwise

			////topLeft
			//pData[pDataStartIndex].pos = groundGeometry.at(z*groundSizeX + x);
			//pData[pDataStartIndex].normal = normals.at(z*groundSizeX + x);
			//pData[pDataStartIndex++].texCoord = D3DXVECTOR2(0,0);
			////topRight
			//pData[pDataStartIndex].pos = groundGeometry.at(z*groundSizeX + x+1);
			//pData[pDataStartIndex].normal = normals.at(z*groundSizeX + x+1);
			//pData[pDataStartIndex++].texCoord = D3DXVECTOR2(1,0);
			////lowerLeft
			//pData[pDataStartIndex].pos = groundGeometry.at((z+1)*groundSizeX + x);
			//pData[pDataStartIndex].normal = normals.at((z+1)*groundSizeX + x);
			//pData[pDataStartIndex++].texCoord = D3DXVECTOR2(0,1);

			////lowerRight
			//pData[pDataStartIndex].pos = groundGeometry.at((z+1)*groundSizeX + x+1);
			//pData[pDataStartIndex].normal = normals.at((z+1)*groundSizeX + x+1);
			//pData[pDataStartIndex++].texCoord = D3DXVECTOR2(1,1);
			////lowerLeft
			//pData[pDataStartIndex].pos = groundGeometry.at((z+1)*groundSizeX + x);
			//pData[pDataStartIndex].normal = normals.at((z+1)*groundSizeX + x);
			//pData[pDataStartIndex++].texCoord = D3DXVECTOR2(0,1);
			////topRight
			//pData[pDataStartIndex].pos = groundGeometry.at(z*groundSizeX + x+1);
			//pData[pDataStartIndex].normal = normals.at(z*groundSizeX + x+1);
			//pData[pDataStartIndex++].texCoord = D3DXVECTOR2(1,0);

			////update buffer index counter
			//bufferIndexAmount += 6;

			//topLeft
			pData[pDataStartIndex].pos = groundGeometry.at(z*groundSizeX + x);
			pData[pDataStartIndex].normal = normals.at(z*groundSizeX + x);
			pData[pDataStartIndex++].texCoord = D3DXVECTOR2(0,0);

			//lowerRight
			pData[pDataStartIndex].pos = groundGeometry.at((z+1)*groundSizeX + x+1);
			pData[pDataStartIndex].normal = normals.at((z+1)*groundSizeX + x+1);
			pData[pDataStartIndex++].texCoord = D3DXVECTOR2(1,1);

			//lowerLeft
			pData[pDataStartIndex].pos = groundGeometry.at((z+1)*groundSizeX + x);
			pData[pDataStartIndex].normal = normals.at((z+1)*groundSizeX + x);
			pData[pDataStartIndex++].texCoord = D3DXVECTOR2(0,1);

			//topLeft
			pData[pDataStartIndex].pos = groundGeometry.at(z*groundSizeX + x);
			pData[pDataStartIndex].normal = normals.at(z*groundSizeX + x);
			pData[pDataStartIndex++].texCoord = D3DXVECTOR2(0,0);

			//topRight
			pData[pDataStartIndex].pos = groundGeometry.at(z*groundSizeX + x+1);
			pData[pDataStartIndex].normal = normals.at(z*groundSizeX + x+1);
			pData[pDataStartIndex++].texCoord = D3DXVECTOR2(1,0);

			//lowerRight
			pData[pDataStartIndex].pos = groundGeometry.at((z+1)*groundSizeX + x+1);
			pData[pDataStartIndex].normal = normals.at((z+1)*groundSizeX + x+1);
			pData[pDataStartIndex++].texCoord = D3DXVECTOR2(1,1);

			//update buffer index counter
			bufferIndexAmount += 6;
		}
	}
}
void Terrain::filter3x3()
{
	float average;
	float sample;
	unsigned int temp;
	for(unsigned int i = 0; i < groundGeometry.size(); i++)
	{
		average = 0.0f;
		sample = 0.0f;
		for(int y = -1; y < 2; y++)
		{
			for(int x = -1; x < 2; x++)
			{
				temp = i+(groundSizeX*y)+x;
				if(temp >= 0 && temp < groundGeometry.size())
				{
					average += groundGeometry.at(temp).y;
					sample++;
				}
			}
		}
		average = average/sample;
		groundGeometry.at(i).y = average;
	}
}
void Terrain::generateNormals(int index1, int index2, int index3)
{
	D3DXVECTOR3 firstvec = D3DXVECTOR3(groundGeometry.at(index2).x - groundGeometry.at(index1).x,
			groundGeometry.at(index2).y - groundGeometry.at(index1).y,
			groundGeometry.at(index2).z - groundGeometry.at(index1).z);

	D3DXVECTOR3 secondvec = D3DXVECTOR3(groundGeometry.at(index2).x - groundGeometry.at(index3).x,
			groundGeometry.at(index2).y - groundGeometry.at(index3).y,
			groundGeometry.at(index2).z - groundGeometry.at(index3).z);

	D3DXVECTOR3 normal;
	D3DXVec3Cross(&normal, &firstvec, &secondvec);
	D3DXVec3Normalize(&normal,&normal);

	normals.at(index1).x += normal.x;
	normals.at(index1).y += normal.y;
	normals.at(index1).z += normal.z;
	D3DXVec3Normalize(&normal,&normal);

	normals.at(index2).x += normal.x;
	normals.at(index2).y += normal.y;
	normals.at(index2).z += normal.z;
	D3DXVec3Normalize(&normal,&normal);

	normals.at(index3).x += normal.x;
	normals.at(index3).y += normal.y;
	normals.at(index3).z += normal.z;
	D3DXVec3Normalize(&normal,&normal);
}
void Terrain::fixNormals()
{
	for(int z = 0; z < groundSizeZ - 1; z++)
		for(int x = 0; x < groundSizeX - 1; x++)
		{
			generateNormals(z*groundSizeX + x+1, z*groundSizeX + x, (z+1)*groundSizeX + x);
			generateNormals((z+1)*groundSizeX + x, (z+1)*groundSizeX + x+1, z*groundSizeX + x+1);
		}
}
void Terrain::draw(ID3D10EffectTechnique* tech)
{
	GraphicsCore *core = GraphicsCore::getInstance();
	useShaderVars();
	core->render(tech, bufferOffset, bufferIndexAmount);
}
Terrain::~Terrain()
{
}
