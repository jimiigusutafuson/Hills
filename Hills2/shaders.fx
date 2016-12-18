//-----------------------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------------------

#define			PT_EMITTER 0
#define			PT_SNOW 1
#define			PT_SMOKE 2

//textures
Texture2D		ground1;
Texture2D		ground2;
Texture2D		ground3;

//particle textures
Texture2D		pTexture;
Texture2D		shadowMap;

//maps
Texture2D		blendMap;

//random
Texture2D		randomSeed;

//depth stencil buffer
Texture2D		depthBufferTex;

//box texture
Texture2D		box;

//-----------------------------------------------------------------------------------------
// In- and Out data
//-----------------------------------------------------------------------------------------
struct VSSceneIn
{
	float3		Pos				: POS;
	float2		Texpos			: TEXPOS;
	float3		Normal			: NORMAL;
};

struct PSSceneIn
{
	float4		Pos				: SV_Position;
	float2		Texpos			: TEXPOS;
	float4		lightIntensity	: LIGHT;
	float2		blendTexC		: BLENDTEXC;
	float4		lPos			: lightPos;
};

struct PSIn
{
	float4		pos				: SV_Position;
	float2		texPos			: TEXPOS;
	float4		lightIntensity	: LIGHT;
};

struct	PSInSoft
{
	float4		pos				: SV_Position;
	float2		texPos			: TEXCOORD0;
	float		alpha			: ALPHA;
	float4		screenTex		: TEXCOORD1;
};

struct SOParticle	//for stream output
{
	float3		pos				: POS;	//position vector
	float2		size			: SIZE; //width and height of particle
	float3		vel				: VEL;	//velocity vector
	float		age				: AGE;	//age vector
	uint		type			: TYPE; //emitter or not? What kind of effect?
};

struct Particle		//for regular particles
{
	float3		pos				: POS;
	float2		size			: SIZE;
};

struct SoftParticle //for rotating soft particles
{
	float3		pos				: POS;
	float2		size			: SIZE;
	float		rot				: ROT;		//rotation
	float		alpha			: ALPHA;
};

struct P_VS_OUT
{
	float3		pos				: POS;
	float2		size			: SIZE;
	uint		type			: TYPE;
	float4		lightIntensity	: LIGHT;
};

struct PSShadowMapIn
{
	float4 pos : SV_POSITION;
}
;
//-----------------------------------------------------------------------------------------
// Constant Buffers
//-----------------------------------------------------------------------------------------
cbuffer cbEveryFrame
{
	
	int2		screenSize;		//width of screen

	float		timeStep;		//time passed since last update
	float		gameTime;		//game time

	float3		ka;				//ambient
	float3		kd;				//diffuse
	float3		ks;				//specular
	float3		cameraPos;
	float3		lightPos;

	float3		pEmitPos;		//particle emitter position

	matrix		mWorld;
	matrix		mView;
	matrix		mViewInv;
	matrix		mProj;
	matrix		mProjInv;

	matrix		mLightView;
};

cbuffer cbOnce
{
	float2 quadTexCoords[4] =
	{
		float2( 1.0f,  1.0f ),
		float2(.0f,  1.0f ),
		float2( 1.0f, .0f ),
		float2(.0f, .0f )
	};
};

//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------
DepthStencilState DisableDepth
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

DepthStencilState EnableDepthTestOnly
{
    DepthEnable = TRUE;
    DepthWriteMask = ZERO;
};

DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};
RasterizerState BackFaceCulling
{
	CullMode = BACK;
};

SamplerState linearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};
SamplerState anisotropicSampler
{
	Filter = ANISOTROPIC;
	AddressU = Wrap;
	AddressV = Wrap;
};
SamplerState clampSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	 AddressU = Clamp;
    AddressV = Clamp;
};
SamplerState pointSampler
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Clamp;
    AddressV = Clamp;
};

BlendState AdditiveBlending
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = ONE;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

BlendState AlphaBlending
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
};

BlendState NoBlend
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_COLOR;
};

float randVec(float offset)
{
	float u = gameTime + offset;
	return randomSeed.SampleLevel(linearSampler, u, 0).x;
}


//-----------------------------------------------------------------------------------------
// VertexShader:
//-----------------------------------------------------------------------------------------
PSSceneIn VSScene(VSSceneIn input)
{
	PSSceneIn output = (PSSceneIn)0;

	matrix mWorldView = mul(mWorld, mView);
	output.Pos = mul( float4(input.Pos,1.0), mWorldView );
	
	float3 tnorm = normalize(mul(float4(input.Normal, .0f ),mWorldView)).xyz;

	float3 s = normalize(mul(float4(lightPos, 1.0f), mView) - output.Pos).xyz;
	float3 lightIntens = kd * max(dot(s, tnorm),0.0) +
						ks * pow(max(dot(normalize(-s + 2 * dot(s, tnorm) * tnorm), 
								normalize(mul(float4(cameraPos, .0f) , mView).xyz - output.Pos.xyz)), 0.0), 80);


	// 1 texture size for every 4th tile
	float2 tile;
	tile.x = (input.Pos.x + 4 * 10) / (4 * 10);
	tile.y = (input.Pos.z - 4 * 10) / (-4 * 10);

	float2 blendTexC;
	blendTexC.x = (input.Pos.x + 256 * 10) / (256 * 10);
	blendTexC.y = (input.Pos.z - 256 * 10) / (-256 * 10);

	//output.Col = float4(input.Col,0);
	output.Pos = mul(output.Pos, mProj );
	output.Texpos = tile;
	output.lightIntensity = float4(lightIntens,1);
	output.blendTexC = blendTexC;
	output.lPos = mul(mul(mul(float4(input.Pos,1.0), mWorld ), mLightView), mProj);
	return output;
}

PSShadowMapIn vsShadowMap(VSSceneIn input)
{
	PSShadowMapIn output = (PSShadowMapIn)0;
	output.pos = mul(mul(mul(float4(input.Pos, 1.0f), mWorld), mLightView), mProj);
	return output;
}

SOParticle StreamOutVS( SOParticle vIn )
{
	return vIn;
}

Particle VSParticle( Particle vIn )
{
	return vIn;
}

P_VS_OUT VSSOParticle( SOParticle vIn )
{
	P_VS_OUT vOut;
	vOut.pos = vIn.pos;
	vOut.size = vIn.size;
	vOut.type = vIn.type;
	vOut.lightIntensity = 1;
	return vOut;
}

SoftParticle VSSoftParticle( SoftParticle vIn )
{
	return vIn;
}

PSIn vsBox(VSSceneIn input)
{
	PSIn output = (PSIn)0;

	matrix mWorldView = mul(mWorld, mView);
	output.pos = mul( float4(input.Pos,1.0), mWorldView );
	
	float3 tnorm = normalize(mul(float4(input.Normal, .0f ),mWorldView)).xyz;

	float3 s = normalize(mul(float4(lightPos, 1.0f), mView) - output.pos).xyz;
	float3 lightIntens = kd * max(dot(s, tnorm),0.0) +
						ks * pow(max(dot(normalize(-s + 2 * dot(s, tnorm) * tnorm), 
								normalize(mul(float4(cameraPos, .0f) , mView).xyz - output.pos.xyz)), 0.0), 80);

	//output.Col = float4(input.Col,0);
	output.pos = mul(output.pos, mProj );
	output.texPos = input.Texpos;
	output.lightIntensity = float4(lightIntens,1);
	
	return output;
}

float4 psBox(PSIn input) : SV_Target
{
	float3 color = box.Sample(anisotropicSampler,input.texPos).rgb;
	return float4(color,1) * float4(ka + input.lightIntensity, 0);
}

float4 psShadowMap(PSShadowMapIn input) : SV_Target
{
	return float4(1,0,0,0);
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{	
	float4 lPos = input.lPos;
	lPos.xyz /= lPos.w;
	float3 blendTexColor = blendMap.Sample(anisotropicSampler,input.blendTexC).rgb;
	
	float inverseTotal = 1.0f / (blendTexColor.x + blendTexColor.y + blendTexColor.z);

	float4 terrainCol1 = ground1.Sample(anisotropicSampler, input.Texpos);
	float4 terrainCol2 = ground2.Sample(anisotropicSampler, input.Texpos);
	float4 terrainCol3 = ground3.Sample(anisotropicSampler, input.Texpos);

	terrainCol1 *= blendTexColor.x * inverseTotal;
	terrainCol2 *= blendTexColor.y * inverseTotal;
	terrainCol3 *= blendTexColor.z * inverseTotal;

	float4 texColor = (terrainCol1 + terrainCol2 + terrainCol3);

	if( lPos.x < -1.0f || lPos.x > 1.0f ||
	    lPos.y < -1.0f || lPos.y > 1.0f ||
	    lPos.z < 0.0f  || lPos.z > 1.0f )
		return texColor * float4(ka + input.lightIntensity, 0);

	float2 shadowMapTexC;
	shadowMapTexC.x = lPos.x * 0.5 + 0.5;
	shadowMapTexC.y = -lPos.y * 0.5 + 0.5;

	float bias = 0.00001;
	float shadowDepth = shadowMap.Sample( anisotropicSampler, shadowMapTexC ).r;
	float s0 = shadowDepth + bias < lPos.z ? 0.0 : 1.0;
	float lightDepth = lPos.z - bias;
	return texColor * float4(ka + input.lightIntensity * s0, 0);
	
}

float4 PSParticle(PSIn input) : SV_Target
{
	float4 color = pTexture.Sample(anisotropicSampler,input.texPos);
	if(color.w == 0)
		discard;
	return color;
}

float4 PSSoftParticle(PSInSoft input) : SV_Target
{
	float2 screenVpos = input.screenTex.xy / input.screenTex.w;
	float inputDepth = input.screenTex.z / input.screenTex.w;
	//converting from (between -1.0 and 1.0) to (between 0.0 and 1.0)
    float2 screenPosition = (screenVpos + 1.0) * .5f; 
	//flipping y coord
    screenPosition.y = 1.0 - screenPosition.y;

	float depth = depthBufferTex.Sample( pointSampler, screenPosition ).r;

	float4 depthviewParticle = mul( float4( screenVpos, inputDepth, 1 ), mProjInv );

	float4 depthViewSample = mul( float4( screenVpos, depth, 1 ), mProjInv );

	float depthDiff = saturate((depthViewSample.z/depthViewSample.w - depthviewParticle.z/depthviewParticle.w) *0.1);

	if( depthDiff == 0 )
		discard;

	float4 color = pTexture.Sample(anisotropicSampler,input.texPos);
	color.w *= input.alpha * depthDiff;
	if(color.w == 0)
		discard;
	return color;
}
//-----------------------------------------------------------------------------------------
// Geometry Shader
//-----------------------------------------------------------------------------------------

//float3		pos				: POS;	//position vector
//	float2		size			: SIZE; //width and height of particle
//	float3		vel				: VEL;	//velocity vector
//	float		age				: AGE;	//age vector
//	uint		type			: TYPE; //emitter or not? What kind of effect?
//this geometry shader creates and destroys partciles
[maxvertexcount(100)]
void StreamOutGS(point SOParticle gIn[1],
				inout PointStream<SOParticle> ptStream)
{
	//gIn[0].age += timeStep;
	gIn[0].age += timeStep;
	if(gIn[0].type == PT_EMITTER)
	{
		if(gIn[0].age > 0.1f )
		{
			for(int i = 1; i < 100; i++)
			{
				float3 rand;
				rand.x = 4000.0f*randVec((float)i/100.0f) - 2000;
				rand.z = 4000.0f*randVec((float)(i+5)/100.0f) - 2000;
				rand.y = 1000.0f;

				SOParticle pOut;
				pOut.pos = gIn[0].pos + rand;
				pOut.size = gIn[0].size;
				pOut.vel.y = 100;
				pOut.vel.x = randVec((float)(i+35)/100.0f)*100;
				pOut.vel.z = randVec((float)(i+55)/100.0f)*100;
				pOut.age = .0f;
				pOut.type = PT_SNOW;

				ptStream.Append(pOut);
			}
			gIn[0].age = .0f;
		}
		ptStream.Append(gIn[0]);
	}
	else
	{
		gIn[0].pos -= timeStep * gIn[0].vel;
		if(gIn[0].pos.y > -1)
			ptStream.Append(gIn[0]);
	}
}

[maxvertexcount(4)]
void DrawSOParticleGs(point P_VS_OUT gIn[1],
				inout TriangleStream<PSIn> triStream)
{
	if(gIn[0].type != PT_EMITTER)
	{
		float3 look		= normalize( cameraPos.xyz - gIn[0].pos );
		float3 right	= normalize( cross( float3( 0, 1, 0 ), look ) );
		float3 up		= cross( look, right );

		//create new world matrix for this particle
		float4x4 pWorld;
		pWorld[0] = float4( right,		0.0f );
		pWorld[1] = float4( up,			0.0f );
		pWorld[2] = float4( look,		0.0f );
		pWorld[3] = float4( gIn[0].pos,	1.0f );

		float4x4 worldViewProj = mul( pWorld, mul(mView, mProj) );

		float halfWidth	= .5f*gIn[0].size.x;
		float halfHeight= .5f*gIn[0].size.y;

		float4 positions[4];
		positions[0] = float4(-halfWidth, -halfHeight, .0f, 1.0f);
		positions[1] = float4( halfWidth, -halfHeight, .0f, 1.0f);
		positions[2] = float4(-halfWidth,  halfHeight, .0f, 1.0f);
		positions[3] = float4( halfWidth,  halfHeight, .0f, 1.0f);

		PSIn gOut;
		[unroll]
		for(int i = 0; i < 4; i++)
		{
			gOut.pos = mul( positions[i], worldViewProj );
			gOut.texPos = quadTexCoords[i];	//predetermined texture corrdinates for a square
			gOut.lightIntensity = 1;	//needs to be fixed
			triStream.Append(gOut);
		}
	}
}
[maxvertexcount(4)]
void DrawSoftParticleGs(point SoftParticle gIn[1],
				inout TriangleStream<PSInSoft> triStream)
{
	float3 look		= normalize( cameraPos.xyz - gIn[0].pos );
	float3 right	= normalize( cross( float3( 0, 1, 0 ), look ) );				
	float3 up		= cross( look, right );
	
	float4x4 pWorld; //create new world matrix for this particle
	pWorld[0] = float4( right,		0.0f );
	pWorld[1] = float4( up,			0.0f );
	pWorld[2] = float4( look,		0.0f );
	pWorld[3] = float4( gIn[0].pos,	1.0f );

	float4x4 worldViewProj = mul( pWorld, mul(mView, mProj) );

	float halfWidth	= .5f*gIn[0].size.x;
	float halfHeight= .5f*gIn[0].size.y;

	float4x4 mrotation; //rotation matrix to make billboards roll (but still facing you)
	mrotation[0] = float4(cos(gIn[0].rot), sin(gIn[0].rot), .0f, .0f);
	mrotation[1] = float4(-sin(gIn[0].rot), cos(gIn[0].rot), .0f, .0f);
	mrotation[2] = float4(.0f, .0f, 1.0f, 0);
	mrotation[3] = float4(.0f, .0f, .0f, 1.0f);

	float4 positions[4];
	positions[0] = float4(-halfWidth, -halfHeight, .0f, 1.0f);
	positions[1] = float4( halfWidth, -halfHeight, .0f, 1.0f);
	positions[2] = float4(-halfWidth,  halfHeight, .0f, 1.0f);
	positions[3] = float4( halfWidth,  halfHeight, .0f, 1.0f);

	PSInSoft gOut;
	[unroll]
	for(int i = 0; i < 4; i++)
	{
		//gOut.pos		= mul(positions[i], worldViewProj );
		gOut.pos		= mul( mul( positions[i], mrotation), worldViewProj );
		gOut.texPos		= quadTexCoords[i];	//predetermined texture corrdinates for a square
		gOut.alpha		= gIn[0].alpha;
		gOut.screenTex	= gOut.pos;//don't perform W dividion here. Will cause depth glitching
		triStream.Append(gOut);
	}
}

[maxvertexcount(4)]
void GSParticle(point Particle gIn[1],
				inout TriangleStream<PSIn> triStream)
{
	float3 look		= normalize( cameraPos.xyz - gIn[0].pos );
	float3 right	= normalize( cross( float3( 0, 1, 0 ), look ) );
	float3 up		= cross( look, right );

	//create new world matrix for this particle
	float4x4 pWorld;
	pWorld[0] = float4( right,		0.0f );
	pWorld[1] = float4( up,			0.0f );
	pWorld[2] = float4( look,		0.0f );
	pWorld[3] = float4( gIn[0].pos,	1.0f );

	float4x4 worldViewProj = mul( pWorld, mul(mView, mProj) );

	float halfWidth	= .5f*gIn[0].size.x;
	float halfHeight= .5f*gIn[0].size.y;

	float4 positions[4];
	positions[0] = float4(-halfWidth, -halfHeight, .0f, 1.0f);
	positions[1] = float4( halfWidth, -halfHeight, .0f, 1.0f);
	positions[2] = float4(-halfWidth,  halfHeight, .0f, 1.0f);
	positions[3] = float4( halfWidth,  halfHeight, .0f, 1.0f);

	PSIn gOut;
	[unroll]
	for(int i = 0; i < 4; i++)
	{
		gOut.pos = mul( positions[i], worldViewProj );
		gOut.texPos = quadTexCoords[i];	//predetermined texture corrdinates for a square
		gOut.lightIntensity = 1;	//needs to be fixed
		triStream.Append(gOut);
	}
}
//shadowmap
[maxvertexcount(4)]
void GSUIShadowMap(point Particle gIn[1],
				inout TriangleStream<PSIn> triStream)
{
	float4 positions[4];
	positions[0] = float4(-1.0f,	.5f,	.0f,	1);
	positions[1] = float4(-1.0f,	1.0f,	.0f,	1);
	positions[2] = float4(-.5f,		.5f,	.0f,	1);
	positions[3] = float4(-.5f,		1.0f,	.0f,	1);

	float2 quadTexCoords[4];
	quadTexCoords[0]	= float2( 1.0f,  1.0f );
	quadTexCoords[1]	= float2( 1.0f, .0f );
	quadTexCoords[2]	= float2(.0f,  1.0f );
	quadTexCoords[3]	= float2(.0f, .0f );

	PSIn gOut;
	[unroll]
	for(int i = 0; i < 4; i++)
	{
		gOut.pos = positions[i];
		gOut.texPos = quadTexCoords[i];	//predetermined texture corrdinates for a square
		gOut.lightIntensity = 1;
		triStream.Append(gOut);
	}
}

//-----------------------------------------------------------------------------------------
// Technique: RenderTextured  
//-----------------------------------------------------------------------------------------
technique10 DrawScene
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
	    
	    SetRasterizerState( BackFaceCulling );
		SetBlendState( NoBlend, float4( .0f, .0f, .0f, .0f ), 0xffffffff );
	    SetDepthStencilState( EnableDepth, 0 );
	    //SetDepthStencilState( DisableDepth, 0 );
	    //SetDepthStencilState( EnableDepthTestOnly, 0 );
    }  
}

GeometryShader gsStreamOut = ConstructGSWithSO( CompileShader( gs_4_0, StreamOutGS() ), "POS.xyz; SIZE.xy; VEL.xyz; AGE.x; TYPE.x");

technique10 StreamOut // for creating and deleting particles
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_0, StreamOutVS() ) );
		SetGeometryShader( gsStreamOut );

		SetPixelShader( NULL ); //disable pixel shader

		SetDepthStencilState( DisableDepth, 0 ); //disable depth buffer
	}
}

technique10 DrawSOParticle
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_0, VSSOParticle() ) );
		SetGeometryShader( CompileShader( gs_4_0, DrawSOParticleGs() ) );
		SetPixelShader( CompileShader( ps_4_0, PSParticle() ) );

		SetRasterizerState( BackFaceCulling );
		SetBlendState( AdditiveBlending, float4( .0f, .0f, .0f, .0f ), 0xffffffff );
	    SetDepthStencilState( EnableDepthTestOnly, 0);
	}
}

technique10 DrawSoftParticle
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_0, VSSoftParticle() ) );
		SetGeometryShader( CompileShader( gs_4_0, DrawSoftParticleGs() ) );
		SetPixelShader( CompileShader( ps_4_0, PSSoftParticle() ) );

		SetRasterizerState( BackFaceCulling );
		SetBlendState( AlphaBlending, float4( .0f, .0f, .0f, .0f ), 0xffffffff );
	    SetDepthStencilState( DisableDepth, 0);
	}
}

technique10 DrawParticle
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_0, VSParticle() ) );
		SetGeometryShader( CompileShader( gs_4_0, GSParticle() ) );
		SetPixelShader( CompileShader( ps_4_0, PSParticle() ) );

		SetRasterizerState( BackFaceCulling );
		SetBlendState( AdditiveBlending, float4( .0f, .0f, .0f, .0f ), 0xffffffff );
	    SetDepthStencilState( EnableDepthTestOnly, 0);
	}
}

technique10 DrawBox
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_0, vsBox() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, psBox() ) );
	    
	    SetRasterizerState( BackFaceCulling );
		SetBlendState( NoBlend, float4( .0f, .0f, .0f, .0f ), 0xffffffff );
	    SetDepthStencilState( EnableDepth, 0 );
	}
}

technique10 DrawUIShadowMap
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_0, VSParticle() ) );
		SetGeometryShader( CompileShader( gs_4_0, GSUIShadowMap() ) );
		SetPixelShader( CompileShader( ps_4_0, PSParticle() ) );

		SetRasterizerState( BackFaceCulling );
		SetBlendState( NoBlend, float4( .0f, .0f, .0f, .0f ), 0xffffffff );
	    SetDepthStencilState( DisableDepth, 0);
	}
}
technique10 DrawShadowMap
{
	pass p0
	{
		SetVertexShader( CompileShader( vs_4_0, vsShadowMap() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, psShadowMap() ) );
	    
	    SetRasterizerState( BackFaceCulling );
		SetBlendState( NoBlend, float4( .0f, .0f, .0f, .0f ), 0xffffffff );
	    SetDepthStencilState( EnableDepth, 0 );
	}
}