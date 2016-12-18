#include "Game.h"
#include <sstream>

Game::Game()
{
	GraphicsCore *core			= GraphicsCore::getInstance();

	flying						= true;

	timeStep					= .0f;
	gameTime					= .0f;
	timeSinceFPSUpdate			= .0f;
	fps							= 0;

	input						= Input::getInstance();

	camera						= new Camera(0,400,0);
	hMapLoader					= new HeightMapLoader();
	terrain						= new Terrain(256,256,10);
	block						= new Block(D3DXVECTOR3(124,200,3));
	hMapLoader->loadRAW(256,256,"heightmap4.raw",2,0);
	terrain->generateFromHeightMap(hMapLoader);

	//------------------particles-----------------
	snow						= new SOParticleEffect(D3DXVECTOR3(10,200,10), core->g_pTechParticleTest, 20000);
	snow->setTexture(core->snowFlakeTexture);

	outOfIdeasParticle			= new OutOfIdeasParticle( D3DXVECTOR3(50,220,-100), core->g_pTechParticle, 0, 10); //next using normal particles will have to start on 10
	outOfIdeasParticle->setTexture(core->outOfIdeasTexture);

	softSmoke = new SoftSmokeParticles(D3DXVECTOR3(10,215,10),core->g_pTechSoftParticle, 0, 20); // next using soft particles will have to start on 20
	softSmoke->setTexture(core->smokeTexture);
	//--------------------------------------------

	pData						= new Vertex[400000]();
	terrain->feedVBuffer(0,pData);
	int bufferSize				= terrain->getBufferAmount();
	block->feedVBuffer(bufferSize, pData);
	bufferSize += block->getBufferAmount();

	D3D10_SUBRESOURCE_DATA staticInitPdata;
    staticInitPdata.pSysMem = pData;
	staticInitPdata.SysMemPitch = 0;
	staticInitPdata.SysMemSlicePitch = 0;
	//vertex buffer for static geometry
	vBufferStatic				= new VertexBuffer(VBUFFER_TYPE_IMMUTABLE, bufferSize,sizeof(Vertex), &staticInitPdata);
	//vertex buffer for particles
	vBufferParticles			= new VertexBuffer(VBUFFER_TYPE_DYNAMIC, 400,
													sizeof(Particle), 0, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//vertex buffer for soft particles
	vBufferSoftParticle			= new VertexBuffer(VBUFFER_TYPE_DYNAMIC, 400,
													sizeof(SoftParticle), 0, D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	sParticleData				= NULL;

	//index data for the soft particles (they're rendered by index, which is sorted)
	indicesSoftParticle			= new unsigned int[400];
	for(int i = 0; i < 400; i++)
	{
		indicesSoftParticle[i] = i;
	}

	D3D10_SUBRESOURCE_DATA iData;
    iData.pSysMem = indicesSoftParticle;
	iData.SysMemPitch = 0;
	iData.SysMemSlicePitch = 0;

	D3D10_BUFFER_DESC				indexBufferDesc;
	indexBufferDesc.Usage			= D3D10_USAGE_DYNAMIC;
	indexBufferDesc.ByteWidth		= sizeof(unsigned int) * 400;
	indexBufferDesc.BindFlags		= D3D10_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags	= D3D10_CPU_ACCESS_WRITE;
	indexBufferDesc.MiscFlags		= 0;

	core->createBuffer(&indexBufferDesc, &iBufferSoftParticle, &iData);

	//the position of this world's sun
	sun							= D3DXVECTOR3(-100000,100000,-100000);
	D3DXVECTOR3	lightViewPos	= block->getPosition() + (sun - block->getPosition())*0.002;
	D3DXMATRIX mLightView;
	D3DXVECTOR3 lightDir = block->getPosition() - sun;
	CustomMath::normalizeVector3(&lightDir);
	
	CustomMath::setLookAtMatrix(&mLightView,&lightViewPos,&block->getPosition(),&D3DXVECTOR3(0,1,0));
	core->setShaderVector3("lightPos", &sun);

	core->setShaderMatrix( "mLightView", &mLightView );

	D3DXMATRIX projInv;
	
	//quick determinant fetch (only works on the projection matrix)
	float det = (-camera->getProjectionMatrix()->_43) * camera->getProjectionMatrix()->_34 *
					camera->getProjectionMatrix()->_22 * camera->getProjectionMatrix()->_11;

	//D3DXMatrixInverse(&projInv, &det, camera->getProjectionMatrix());
	CustomMath::createInverseMatrix(&projInv, &det, camera->getProjectionMatrix());

	core->setShaderMatrix("mProjInv", &projInv);
}
void Game::update(float dt)
{
	timeStep = dt;
	gameTime += dt;
	timeSinceFPSUpdate += dt;
	if(timeSinceFPSUpdate >= 1)
	{
		timeSinceFPSUpdate--;
		fps = (int)(1.0 / dt + 0.5);
	}
	input->update();

	D3DXVECTOR3 movement(0,0,0);

	if(input->getKeyState()[DIK_W] & 0x80)
		movement.z++;
	if(input->getKeyState()[DIK_S] & 0x80)
		movement.z--;
	if(input->getKeyState()[DIK_A] & 0x80)
		movement.x++;
	if(input->getKeyState()[DIK_D] & 0x80)
		movement.x--;
	if(input->getKeyState()[DIK_SPACE] & 0x80)
		movement.y++;
	if(input->getKeyState()[DIK_LCONTROL] & 0x80)
		movement.y--;
	if(!(movement.x == 0 && movement.y == 0 && movement.z == 0))
		CustomMath::normalizeVector3(&movement);
	if(flying)
		camera->fly(movement*timeStep*100);
	else
	{
		camera->move(movement*timeStep*100);

		camera->setHeight( terrain->getHeightAtPosition( camera->getPosition() ) + PLAYERHEIGHT );

	}
	camera->rotate((float)input->getDeltaX(),(float)input->getDeltaY(),0);

	//particle updates------------------------------------------------------------------------------
	softSmoke->update(dt);
	outOfIdeasParticle->update(dt);
	//----------------------------------------------------------------------------------------------
}
void Game::draw()
{
	GraphicsCore *core = GraphicsCore::getInstance();

	core->useVertexBuffer(vBufferStatic);
	core->updateGameTime(gameTime);
	core->updateTimeStep(timeStep);
	core->useInputLayout(core->g_pVertexLayout);

	//-------------------------------------------------------------------------------------------------
	//Shadow Map---------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------
	core->setRenderTarget(0,0,core->g_pShadowMapView);
	core->ClearDepthStencilView(core->g_pShadowMapView);
	//terrain->draw(core->g_pTechDrawShadowMap);
	block->draw(core->g_pTechDrawShadowMap);

	//-------------------------------------------------------------------------------------------------
	//Static Objects---------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------
	core->setDepthBufferAsShaderVar(false);
	core->useResource(core->g_pShadowMapResource, "shadowMap");
	terrain->draw(core->g_pTechRenderScene);
	block->draw(core->g_pTechBox);

	//-------------------------------------------------------------------------------------------------
	//stream out particles-----------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------

	core->useInputLayout(core->g_pSOParticleLayout);
	snow->update();	//update particles with streamout
	
	//core->useVertexBuffer(particleBuffer);
	snow->useTexture();
	snow->draw();



	//-------------------------------------------------------------------------------------------------
	//normal particles---------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------

	core->useVertexBuffer(vBufferParticles);

	//normal particle buffer mapping------------------------------------------------------------------
	vBufferParticles->actualBuffer->Map(  D3D10_MAP_WRITE_DISCARD, 0, reinterpret_cast< void** >((void**)&particleData) );

	outOfIdeasParticle->updateBuffer(particleData);

	vBufferParticles->actualBuffer->Unmap();

	outOfIdeasParticle->useTexture();
	outOfIdeasParticle->draw();






	//-------------------------------------------------------------------------------------------------
	//soft particles-----------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------
	core->useVertexBuffer(vBufferSoftParticle);
	core->useIndexBuffer(iBufferSoftParticle);

	//soft particle buffer mapping------------------------------------------------------------------
	vBufferSoftParticle->actualBuffer->Map(  D3D10_MAP_WRITE_DISCARD, 0, reinterpret_cast< void** >((void**)&sParticleData) );

	softSmoke->updateBuffer(sParticleData);

	vBufferSoftParticle->actualBuffer->Unmap();
	//sort the indices of the soft smoke particle effect
	CustomMath::bubbleSort(indicesSoftParticle, sParticleData, softSmoke->getStartIndex(), softSmoke->getActualAmount(), &camera->getPosition() );

	for(unsigned int k = softSmoke->getStartIndex(); k < softSmoke->getActualAmount(); k++)
		cout << indicesSoftParticle[k] << " ";
	cout << "\n";

	//map new indices
	if( FAILED( iBufferSoftParticle->Map( D3D10_MAP_WRITE_DISCARD, 0, reinterpret_cast< void** >((void **)&newmap) ) ) )
	{
		MessageBox( 0, "Failed to map index buffer", "Buffer update error", 0 );
		return;
	}

	memcpy(newmap, indicesSoftParticle, sizeof(unsigned int)*softSmoke->getActualAmount());

	iBufferSoftParticle->Unmap();

	core->useInputLayout(core->g_pSoftParticleLayout);

	core->setDepthBufferAsShaderVar(true);
	softSmoke->useTexture();
	softSmoke->draw();







	//-------------------------------------------------------------------------------------------------
	//UI-----------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------
	core->useVertexBuffer(vBufferParticles);
	core->useResource(core->g_pShadowMapResource, "pTexture");
	core->render(core->g_pTechDrawUIShadowMap,0,1);

	RECT t, t2;
	t.top = 100;
	t.bottom =  150;
	t.left =100;
	t.right =200;

	t2.top = 100;
	t2.bottom = 150;
	t2.left = 300;
	t2.right = 600;

	std::ostringstream buffFps, buffX, buffY, buffZ;
			
	buffFps << fps;
	buffX << camera->getPosition().x;
	buffY << camera->getPosition().y;
	buffZ << camera->getPosition().z;

	string text;
	text += "FPS: " + buffFps.str();
	core->font->DrawTextA(NULL, text.c_str(), -1, &t, DT_LEFT | DT_CENTER , D3DXCOLOR(1, 1, 1, 1.0f));

	string text2;
	text2 += "x: " + buffX.str() + ", y: " + buffY.str() + ", z: " + buffZ.str();
	core->font->DrawTextA(NULL, text2.c_str(), -1, &t2, DT_LEFT | DT_CENTER , D3DXCOLOR(1, 1, 1, 1.0f));
}
Game::~Game()
{
	SAFE_DELETE(vBufferStatic);
	SAFE_DELETE(vBufferSoftParticle);
	SAFE_DELETE(vBufferParticles);

	SAFE_RELEASE(iBufferSoftParticle);
	SAFE_DELETE(indicesSoftParticle);
	SAFE_DELETE(pData);

	SAFE_DELETE(snow);
	SAFE_DELETE(softSmoke);
	SAFE_DELETE(outOfIdeasParticle);

	SAFE_DELETE(camera);
	SAFE_DELETE(terrain);
	SAFE_DELETE(hMapLoader);
	
}
