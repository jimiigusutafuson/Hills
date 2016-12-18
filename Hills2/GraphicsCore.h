#pragma once
#include "stdafx.h"
#include "VertexBuffer.h"
class GraphicsCore
{
private:
	bool						failed;
	static GraphicsCore			*_this;
	D3D10_DRIVER_TYPE			 g_driverType;
	ID3D10Device				*g_pd3dDevice;
	IDXGISwapChain				*g_pSwapChain;
	ID3D10RenderTargetView		*g_pRenderTargetView;

	//depth stencil view
	ID3D10Texture2D				*g_pDepthStencil;			//texture
	
	//shadowmap
	ID3D10Texture2D				*g_pShadowMap;				//texture
	
	

	ID3D10ShaderResourceView	*sRView;

	HWND						hWnd;

	ID3D10Effect				*g_pEffect;

	HRESULT						InitDevice();

	GraphicsCore(HWND g_hWnd);
public:
	bool						hasFailed() { return failed; }
	static GraphicsCore*		getInstance();
	static GraphicsCore*		createInstance(HWND g_hWnd);

	//layouts
	ID3D10InputLayout			*g_pVertexLayout;
	ID3D10InputLayout			*g_pSOParticleLayout;
	ID3D10InputLayout			*g_pParticleLayout;
	ID3D10InputLayout			*g_pSoftParticleLayout;

	//resources
	ID3D10ShaderResourceView	*smokeTexture;
	ID3D10ShaderResourceView	*snowFlakeTexture;
	ID3D10ShaderResourceView	*diceTexture;
	ID3D10ShaderResourceView	*outOfIdeasTexture;

	ID3D10ShaderResourceView	*g_pDepthStencilResource;	//depth stencil view
	ID3D10DepthStencilView		*g_pDepthStencilView;

	ID3D10ShaderResourceView	*g_pShadowMapResource;		//shadowmap
	ID3D10DepthStencilView		*g_pShadowMapView;

	//techniques
	ID3D10EffectTechnique		*g_pTechRenderScene;
	ID3D10EffectTechnique		*g_pTechParticleTest;
	ID3D10EffectTechnique		*g_pTechStreamOutSnow;
	ID3D10EffectTechnique		*g_pTechSoftParticle;
	ID3D10EffectTechnique		*g_pTechParticle;
	ID3D10EffectTechnique		*g_pTechBox;
	ID3D10EffectTechnique		*g_pTechDrawUIShadowMap;
	ID3D10EffectTechnique		*g_pTechDrawShadowMap;

	ID3DX10Font					*font;

	//vertex buffer
	HRESULT						createVertexBuffer(VertexBuffer *vertexBuffer, const D3D10_SUBRESOURCE_DATA *pData = 0);
	HRESULT						createBuffer(D3D10_BUFFER_DESC *desc, ID3D10Buffer **buffer, const D3D10_SUBRESOURCE_DATA *pData = 0);

	void						useVertexBuffer(VertexBuffer *vertexBuffer);
	void						useIndexBuffer(ID3D10Buffer *indexBuffer, DXGI_FORMAT format = DXGI_FORMAT_R32_UINT, UINT offset = 0);
	void						setShaderVector3(LPCSTR varName, D3DXVECTOR3 *vector);
	void						setShaderMatrix(LPCSTR varName, D3DMATRIX *matrix);
	//rendering
	void						clearRenderTarget();
	void						swapChain();

	void						useWorldMatrix(D3DXMATRIX m);
	void						useViewMatrix(D3DXMATRIX m);
	void						useProjectionMatrix(D3DXMATRIX m);
	void						useKaKdKs(D3DXVECTOR3 ka, D3DXVECTOR3 kd, D3DXVECTOR3 ks);
	void						useResource(ID3D10ShaderResourceView *resource, const char *name);
	void						useInputLayout(ID3D10InputLayout *layout);
	void						setDepthBufferAsShaderVar(bool foo);

	void						updateTimeStep(float dt);
	void						updateGameTime(float gametime);
	void						render(ID3D10EffectTechnique* tech, UINT offset, UINT numberOfVertices);
	void						renderIndexed(ID3D10EffectTechnique* tech, UINT indexOffset, UINT vertexOffset, UINT numberOfIndices);
	void						renderSOParticles(ID3D10EffectTechnique* tech);
	void						updateParticles(ID3D10EffectTechnique *tech, UINT offset, UINT numberOfParticles,
												VertexBuffer *streamOutBuffer, VertexBuffer *drawBuffer, UINT stride);
	void						updateParticlesFirstTime(ID3D10EffectTechnique *tech, UINT offset, VertexBuffer *initBuffer,
														VertexBuffer *drawBuffer, UINT stride);
	void						updateIndexBuffer(ID3D10Buffer **iBuffer, UINT amount, UINT startIndex, UINT *indices);

	void						setRenderTarget(UINT numView, ID3D10RenderTargetView *const *ppRenderTargetView, ID3D10DepthStencilView *stencilView);
	void						ClearDepthStencilView(ID3D10DepthStencilView * view);

	~GraphicsCore();
};

