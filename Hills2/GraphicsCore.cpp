#include "GraphicsCore.h"

GraphicsCore *GraphicsCore::_this = NULL;
GraphicsCore::GraphicsCore(HWND g_hWnd)
{
	failed					= false; //success as default
	g_driverType			= D3D10_DRIVER_TYPE_NULL;
	g_pd3dDevice			= NULL;
	g_pSwapChain			= NULL;
	g_pRenderTargetView		= NULL;
	g_pDepthStencil			= NULL;
	g_pDepthStencilView		= NULL;
	g_pDepthStencilResource = NULL;
	sRView					= NULL;

	hWnd					= g_hWnd;
	if( FAILED( InitDevice() ) )
		failed = true;
}

GraphicsCore* GraphicsCore::createInstance(HWND g_hWnd)
{
	if(!_this)
		_this = new GraphicsCore(g_hWnd);
	return _this;
}

GraphicsCore* GraphicsCore::getInstance()
{
	if(_this)
		return _this;
	else return NULL;
}

HRESULT GraphicsCore::InitDevice()
{
	HRESULT hr = S_OK;;

	RECT rc;
	GetClientRect( hWnd, &rc );
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;

//#ifdef _DEBUG
//	createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
//#endif

	D3D10_DRIVER_TYPE driverTypes[] = 
	{
		D3D10_DRIVER_TYPE_HARDWARE,
		D3D10_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D10CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags,
		D3D10_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice );
		if( SUCCEEDED( hr ) )
			break;
	}
	if( FAILED(hr) )
	{
		MessageBox(0, "Error creating device and swapchain!", 0, 0);
		return hr;
	}

	// Create a render target view
	ID3D10Texture2D* pBackBuffer;
	hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (LPVOID*)&pBackBuffer );
	if( FAILED(hr) )
	{
		MessageBox(0, "Error getting backbuffer!", 0, 0);
		return hr;
	}

	hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
	SAFE_RELEASE(pBackBuffer);
	if( FAILED(hr) )
	{
		MessageBox(0, "Error creating render target view!", 0, 0);
		return hr;
	}

	// Create depth stencil texture
	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	//descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	//we need it to be typeless to later set it as two different formats (DXGI_FORMAT_D32_FLOAT and DXGI_FORMAT_R32_FLOAT)
	descDepth.Format = DXGI_FORMAT_R32_TYPELESS;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D10_USAGE_DEFAULT;
	//descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D( &descDepth, NULL, &g_pDepthStencil );
	if( FAILED(hr) )
	{
		MessageBox(0, "Error creating the depth stencil texture!", 0, 0);
		return hr;
	}
	//shadowmap
	hr = g_pd3dDevice->CreateTexture2D( &descDepth, NULL, &g_pShadowMap );
	if( FAILED(hr) )
	{
		MessageBox(0, "Error creating the Shadow map texture!", 0, 0);
		return hr;
	}


	// Create the depth stencil view
	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	//descDSV.Format = descDepth.Format;
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView( g_pDepthStencil, &descDSV, &g_pDepthStencilView );
	if( FAILED(hr) )
	{
		MessageBox(0, "Error creating depth stencil view!", 0, 0);
		return hr;
	}
	//shadowMap
	hr = g_pd3dDevice->CreateDepthStencilView( g_pShadowMap, &descDSV, &g_pShadowMapView );
	if( FAILED(hr) )
	{
		MessageBox(0, "Error creating Shadow map view!", 0, 0);
		return hr;
	}

	//create the depth stencil as a shader resource
	D3D10_SHADER_RESOURCE_VIEW_DESC descSRV;
    descSRV.Format = DXGI_FORMAT_R32_FLOAT;	// Change to R32_FLOAT from typeless
    descSRV.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    descSRV.Texture2D.MostDetailedMip = 0;
	descSRV.Texture2D.MipLevels = 1;
    hr = g_pd3dDevice->CreateShaderResourceView( g_pDepthStencil, &descSRV, &g_pDepthStencilResource );
	if( FAILED(hr) )
	{
		MessageBox(0, "Error creating depth stencil shader resource!", 0, 0);
		return hr;
	}
	//shadowMap
	hr = g_pd3dDevice->CreateShaderResourceView( g_pShadowMap, &descSRV, &g_pShadowMapResource );
	if( FAILED(hr) )
	{
		MessageBox(0, "Error creating depth Shadow map resource!", 0, 0);
		return hr;
	}


	g_pd3dDevice->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );
	// Setup the viewport
	D3D10_VIEWPORT vp;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pd3dDevice->RSSetViewports( 1, &vp );
	//Init shader effect
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
	if(FAILED(D3DX10CreateEffectFromFile(	"shaders.fx",
											NULL,
											NULL,
											"fx_4_0",
											dwShaderFlags,
											0,
											g_pd3dDevice,
											NULL,
											NULL,
											&g_pEffect,
											NULL,
											NULL)))
	{
		MessageBox(0, "Error compiling shader!", "Shader error!", 0);
		return E_FAIL;
	}
	//get the techniques from the shader
	//createTechnique(g_pTechRenderScene, "DrawScene");
	g_pTechRenderScene		= g_pEffect->GetTechniqueByName("DrawScene");
	g_pTechParticleTest		= g_pEffect->GetTechniqueByName("DrawSOParticle");
	g_pTechStreamOutSnow	= g_pEffect->GetTechniqueByName("StreamOut");
	g_pTechSoftParticle		= g_pEffect->GetTechniqueByName("DrawSoftParticle");
	g_pTechParticle			= g_pEffect->GetTechniqueByName("DrawParticle");
	g_pTechBox				= g_pEffect->GetTechniqueByName("DrawBox");
	g_pTechDrawUIShadowMap	= g_pEffect->GetTechniqueByName("DrawUIShadowMap");
	g_pTechDrawShadowMap	= g_pEffect->GetTechniqueByName("DrawShadowMap");
	
	
	D3D10_PASS_DESC PassDesc;
	g_pTechRenderScene->GetPassByIndex(0)->GetDesc(&PassDesc);
	//
	const D3D10_INPUT_ELEMENT_DESC vertexLayout[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXPOS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float)*3, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float)*5,  D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	if( FAILED(g_pd3dDevice->CreateInputLayout(vertexLayout,
									sizeof(vertexLayout) / sizeof(D3D10_INPUT_ELEMENT_DESC),
									PassDesc.pIAInputSignature,
									PassDesc.IAInputSignatureSize,
									&g_pVertexLayout ) ) )
	{
		MessageBox( 0, "Unable to create Input Layout for vertices", "VB Error", 0 );
		return E_FAIL;
	}

	//----------------------------Stream Out Particle----------------------------------
	g_pTechParticleTest->GetPassByIndex(0)->GetDesc(&PassDesc);

	const D3D10_INPUT_ELEMENT_DESC sOparticleLayout[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float)*3, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "VEL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float)*5,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "AGE", 0, DXGI_FORMAT_R32_FLOAT, 0, sizeof(float)*8,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TYPE", 0, DXGI_FORMAT_R32_UINT, 0, sizeof(float)*9,  D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};
	if( FAILED( g_pd3dDevice->CreateInputLayout(sOparticleLayout,
									sizeof(sOparticleLayout) / sizeof(D3D10_INPUT_ELEMENT_DESC),
									PassDesc.pIAInputSignature,
									PassDesc.IAInputSignatureSize,
									&g_pSOParticleLayout ) ) )
	{
		MessageBox( 0, "Unable to create Input Layout for Stream Out particles", "VB Error", 0 );
		return E_FAIL;
	}

	//-------------------------------Particle--------------------------------------------
	//g_pParticleLayout

	g_pTechParticle->GetPassByIndex(0)->GetDesc(&PassDesc);

	const D3D10_INPUT_ELEMENT_DESC particleLayout[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float)*3, D3D10_INPUT_PER_VERTEX_DATA, 0 },
	};
	if( FAILED( g_pd3dDevice->CreateInputLayout(particleLayout,
									sizeof(particleLayout) / sizeof(D3D10_INPUT_ELEMENT_DESC),
									PassDesc.pIAInputSignature,
									PassDesc.IAInputSignatureSize,
									&g_pParticleLayout ) ) )
	{
		MessageBox( 0, "Unable to create Input Layout for particles", "VB Error", 0 );
		return E_FAIL;
	}

	//-----------------------------Soft Particle-----------------------------------------

	g_pTechSoftParticle->GetPassByIndex(0)->GetDesc(&PassDesc);

	const D3D10_INPUT_ELEMENT_DESC softParticleLayout[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float)*3, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "ROT", 0, DXGI_FORMAT_R32_FLOAT, 0, sizeof(float)*5,  D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "ALPHA", 0, DXGI_FORMAT_R32_FLOAT, 0, sizeof(float)*6,  D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};
	if( FAILED( g_pd3dDevice->CreateInputLayout(softParticleLayout,
									sizeof(softParticleLayout) / sizeof(D3D10_INPUT_ELEMENT_DESC),
									PassDesc.pIAInputSignature,
									PassDesc.IAInputSignatureSize,
									&g_pSoftParticleLayout ) ) )
	{
		MessageBox( 0, "Unable to create Input Layout for particles", "VB Error", 0 );
		return E_FAIL;
	}


	
	int screenSize[] = { width, height };
	g_pEffect->GetVariableByName( "screenSize" )->AsVector()->SetIntVector(screenSize);

	//-------------------------------------------------------------------------------------

	D3DX10CreateShaderResourceViewFromFile( g_pd3dDevice, "dirt.png", NULL, NULL, &sRView, NULL );
	g_pEffect->GetVariableByName("ground1")->AsShaderResource()->SetResource(sRView);
	D3DX10CreateShaderResourceViewFromFile( g_pd3dDevice, "snow.png", NULL, NULL, &sRView, NULL );
	g_pEffect->GetVariableByName("ground2")->AsShaderResource()->SetResource(sRView);
	D3DX10CreateShaderResourceViewFromFile( g_pd3dDevice, "rock.png", NULL, NULL, &sRView, NULL );
	g_pEffect->GetVariableByName("ground3")->AsShaderResource()->SetResource(sRView);
	D3DX10CreateShaderResourceViewFromFile( g_pd3dDevice, "blendMap3.png", NULL, NULL, &sRView, NULL );
	g_pEffect->GetVariableByName("blendMap")->AsShaderResource()->SetResource(sRView);
	D3DX10CreateShaderResourceViewFromFile( g_pd3dDevice, "dice.png", NULL, NULL, &diceTexture, NULL );
	D3DX10CreateShaderResourceViewFromFile( g_pd3dDevice, "snowflake.png", NULL, NULL, &snowFlakeTexture, NULL );
	D3DX10CreateShaderResourceViewFromFile( g_pd3dDevice, "smoke2.png", NULL, NULL, &smokeTexture, NULL );
	D3DX10CreateShaderResourceViewFromFile( g_pd3dDevice, "particle.png", NULL, NULL, &outOfIdeasTexture, NULL );

	D3DX10CreateShaderResourceViewFromFile( g_pd3dDevice, "seed.png", NULL, NULL, &sRView, NULL );
	g_pEffect->GetVariableByName("randomSeed")->AsShaderResource()->SetResource(sRView);

	D3DX10CreateShaderResourceViewFromFile( g_pd3dDevice, "combined.png", NULL, NULL, &sRView, NULL );
	g_pEffect->GetVariableByName("box")->AsShaderResource()->SetResource(sRView);

	//--------------------------------------fonts and UI---------------------------------------------
	D3DX10CreateFont(g_pd3dDevice, 16, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS,
									ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &font);
	
	return S_OK;
}

HRESULT GraphicsCore::createVertexBuffer(VertexBuffer *vertexBuffer, const D3D10_SUBRESOURCE_DATA *pData)
{
	if( FAILED( g_pd3dDevice->CreateBuffer( &vertexBuffer->bufferDesc, pData, &vertexBuffer->actualBuffer ) ) )
	{
		MessageBox( 0, "Unable to create Vertex Buffer", "Vertex Buffer Creation Error", 0 );
		return E_FAIL;
	}
	return S_OK;
}

HRESULT GraphicsCore::createBuffer(D3D10_BUFFER_DESC *desc, ID3D10Buffer **buffer, const D3D10_SUBRESOURCE_DATA *pData)
{
	if( FAILED( g_pd3dDevice->CreateBuffer( desc, pData, buffer ) ) )
	{
		MessageBox( 0, "Unable to create Buffer", "Buffer Creation Error", 0 );
		return E_FAIL;
	}
	return S_OK;
}

void GraphicsCore::useVertexBuffer(VertexBuffer *vertexBuffer)
{
	//Set the layouts and topology
	g_pd3dDevice->IASetPrimitiveTopology( vertexBuffer->topology );
	g_pd3dDevice->IASetVertexBuffers( 0, 1, &vertexBuffer->actualBuffer, &vertexBuffer->vertexStructSize, &vertexBuffer->offset );
}
void GraphicsCore::useIndexBuffer(ID3D10Buffer *indexBuffer, DXGI_FORMAT format, UINT offset)
{
	g_pd3dDevice->IASetIndexBuffer(indexBuffer,format, offset);
}
void GraphicsCore::useWorldMatrix(D3DXMATRIX m)
{
	g_pEffect->GetVariableByName("mWorld")->AsMatrix()->SetMatrix((float*)&m);
}
void GraphicsCore::useViewMatrix(D3DXMATRIX m)
{
	g_pEffect->GetVariableByName("mView")->AsMatrix()->SetMatrix((float*)&m);
}
void GraphicsCore::useProjectionMatrix(D3DXMATRIX m)
{
	g_pEffect->GetVariableByName("mProj")->AsMatrix()->SetMatrix((float*)&m);
}
void GraphicsCore::useKaKdKs(D3DXVECTOR3 ka, D3DXVECTOR3 kd, D3DXVECTOR3 ks)
{
	g_pEffect->GetVariableByName( "ka" )->AsVector()->SetFloatVector( (float*)&ka );
	g_pEffect->GetVariableByName( "kd" )->AsVector()->SetFloatVector( (float*)&kd );
	g_pEffect->GetVariableByName( "ks" )->AsVector()->SetFloatVector( (float*)&ks );
}

void GraphicsCore::useResource(ID3D10ShaderResourceView *resource, const char *name)
{
	g_pEffect->GetVariableByName(name)->AsShaderResource()->SetResource(resource);
}

void GraphicsCore::useInputLayout(ID3D10InputLayout *layout)
{
	g_pd3dDevice->IASetInputLayout( layout );
}

void GraphicsCore::updateTimeStep(float dt)
{
	g_pEffect->GetVariableByName( "timeStep" )->AsScalar()->SetFloat( dt );
}

void GraphicsCore::updateGameTime(float gameTime)
{
	g_pEffect->GetVariableByName( "gameTime" )->AsScalar()->SetFloat( gameTime );
}

void GraphicsCore::setShaderVector3(LPCSTR varName, D3DXVECTOR3 *vector)
{
	g_pEffect->GetVariableByName( varName )->AsVector()->SetFloatVector( (float*)vector );
}
void GraphicsCore::setShaderMatrix(LPCSTR varName, D3DMATRIX *matrix)
{
	g_pEffect->GetVariableByName( varName )->AsMatrix()->SetMatrix((float*)matrix);
}
void GraphicsCore::setDepthBufferAsShaderVar(bool foo)
{
	if(foo)
	{
		g_pd3dDevice->OMSetRenderTargets( 1, &g_pRenderTargetView, NULL );
		
		g_pEffect->GetVariableByName("depthBufferTex")->AsShaderResource()->SetResource(g_pDepthStencilResource);
	}
	else
	{
		ID3D10ShaderResourceView *const pSRV[1] = {NULL};
		g_pd3dDevice->PSSetShaderResources(1, 1, pSRV);
		g_pd3dDevice->OMSetRenderTargets( 1, &g_pRenderTargetView, g_pDepthStencilView );
	}
}
//called from the object's draw.
void GraphicsCore::render(ID3D10EffectTechnique* tech, UINT offset, UINT numberOfVertices)
{
	D3D10_TECHNIQUE_DESC techDesc;
	tech->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		tech->GetPassByIndex( p )->Apply(0);
		g_pd3dDevice->Draw(numberOfVertices, offset);
	}
}

void GraphicsCore::renderIndexed(ID3D10EffectTechnique* tech, UINT indexOffset, UINT vertexOffset, UINT numberOfIndices)
{
	D3D10_TECHNIQUE_DESC techDesc;
	tech->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		tech->GetPassByIndex( p )->Apply(0);
		g_pd3dDevice->DrawIndexed(numberOfIndices, indexOffset, vertexOffset);
	}
}
void GraphicsCore::renderSOParticles(ID3D10EffectTechnique* tech)
{
	D3D10_TECHNIQUE_DESC techDesc;

	tech->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		tech->GetPassByIndex( p )->Apply(0);
		//g_pd3dDevice->Draw(numberOfParticles, offset);
		g_pd3dDevice->DrawAuto();
	}
}
void GraphicsCore::updateParticlesFirstTime(ID3D10EffectTechnique *tech, UINT offset, VertexBuffer *initBuffer,
											VertexBuffer *drawBuffer, UINT stride)
{
	useVertexBuffer(initBuffer);

	g_pd3dDevice->SOSetTargets(1,&drawBuffer->actualBuffer, &offset);				//streamOutBuffer "receives"
	D3D10_TECHNIQUE_DESC techDesc;
	tech->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		tech->GetPassByIndex( p )->Apply(0);
		g_pd3dDevice->Draw(1, 0);
	}

	//unbind buffer
	ID3D10Buffer *bufferArray[1] = {0};
	g_pd3dDevice->SOSetTargets(1, bufferArray, &offset);
}
void GraphicsCore::updateParticles(ID3D10EffectTechnique *tech, UINT offset, UINT numberOfParticles,
									VertexBuffer *streamOutBuffer, VertexBuffer *drawBuffer, UINT stride)
{
	useVertexBuffer(drawBuffer);

	g_pd3dDevice->SOSetTargets(1,&streamOutBuffer->actualBuffer, &offset);				//streamOutBuffer "receives"

	D3D10_TECHNIQUE_DESC techDesc;
	tech->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		tech->GetPassByIndex( p )->Apply(0);
		//g_pd3dDevice->Draw(numberOfParticles, offset);
		g_pd3dDevice->DrawAuto();
	}

	//unbind buffer
	ID3D10Buffer *bufferArray[1] = {0};
	g_pd3dDevice->SOSetTargets(1, bufferArray, &offset);

	//std::swap(drawBuffer, streamOutBuffer);	//swap buffers so drawBuffer still points to the updated buffer
}

void GraphicsCore::updateIndexBuffer(ID3D10Buffer **iBuffer, UINT amount, UINT startIndex, UINT *indices)
{
	//g_pd3dDevice->UpdateSubresource(0,NULL,
}
void GraphicsCore::clearRenderTarget()
{
	//clear render target
	g_pd3dDevice->ClearRenderTargetView( g_pRenderTargetView, D3DXVECTOR4(.2f,.1f,.2f,1.0f) );

	//clear depth info
	g_pd3dDevice->ClearDepthStencilView( g_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0 );
}

void GraphicsCore::swapChain()
{
	//swap the chain
	g_pSwapChain->Present( 0, 0 );
}

void GraphicsCore::setRenderTarget(UINT numView, ID3D10RenderTargetView *const *ppRenderTargetView, ID3D10DepthStencilView *stencilView)
{
	g_pd3dDevice->OMSetRenderTargets(numView ,ppRenderTargetView, stencilView);
}

void GraphicsCore::ClearDepthStencilView(ID3D10DepthStencilView * view)
{
	g_pd3dDevice->ClearDepthStencilView( view, D3D10_CLEAR_DEPTH, 1.0f, 0 );
}

GraphicsCore::~GraphicsCore()
{
	SAFE_RELEASE(g_pd3dDevice);
	SAFE_RELEASE(g_pSwapChain);
	SAFE_RELEASE(g_pRenderTargetView);
	SAFE_RELEASE(g_pDepthStencil);
	SAFE_RELEASE(g_pDepthStencilView);
	SAFE_RELEASE(sRView);
	SAFE_RELEASE(g_pEffect);
	SAFE_RELEASE(g_pVertexLayout);
	SAFE_RELEASE(g_pSOParticleLayout);
	SAFE_RELEASE(g_pParticleLayout);
}
