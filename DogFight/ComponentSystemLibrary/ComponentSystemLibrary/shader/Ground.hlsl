Texture2D		g_Tex : register(t0);					// テクスチャ
TextureCube<float4>	SkyMap: register(t1);
TextureCube		EnviromentMap:register(t1);				// 環境マップ
SamplerState	g_SamplerLinear : register(s0);			// サンプラー

cbuffer ConstantBufferWorld : register(b0)		// ワールド変換行列
{
	matrix World;
}

cbuffer ConstantBufferProjection : register(b1) // ワールド変換行列
{
	matrix ViewProjection;
}

cbuffer ConstantBufferProjection : register(b2) // ワールド変換行列
{
	matrix Projection;
}

cbuffer OceanBuffer : register(b3) // ワールド変換行列
{
	float size;		// サイズ
	float uv;		// 動かすかも？よう
	float widHeiMapSize;
	float dummy2;
}

cbuffer ConstantBufferLight : register(b4)
{
	float4 LightDirection;			// 光の方向
	float4 EyePos;					// 視点位置
	float4 Ambient;
}

struct VSParticleOut
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

struct GSParticleOut
{
	float4 position : SV_Position;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	float3 texcoord2: TEXCOORD1;
	float4 WPos		: TEXCOORD2;
	float fogFactor : FOG;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------

VSParticleOut GroundVS(uint ID : SV_VertexID)
{
	VSParticleOut Out = (VSParticleOut)0;
	Out.color = float4(1, 1, 1, 1);
	Out.position = 0;
	Out.position.x = ID % (int)size;
	Out.position.y = ID / (int)size % (int)(size * size);

	Out.texcoord = Out.position % 2;
	return Out;
}

//--------------------------------------------------------------------------------------
// ジオメトリシェーダー
//--------------------------------------------------------------------------------------

//static const float2 g_positions[4] = { float2(-1, 1), float2(1, 1), float2(-1, -1), float2(1, -1) };
static const float2 g_positions[4] = { float2(-0.5, 0.5), float2(0.5, 0.5), float2(-0.5, -0.5), float2(0.5, -0.5) };
static const float2 g_texcoords[4] = { float2(0, 1), float2(1, 1), float2(0, 0), float2(1, 0) };

[maxvertexcount(4)]
void GroundGS(point VSParticleOut In[1], inout TriangleStream<GSParticleOut> SpriteStream)
{
	[unroll]
	for (int i = 0; i < 4; i++)
	{
		GSParticleOut Out = (GSParticleOut)0;
		float4 position = float4(In[0].position.x, In[0].position.y, 0, 1) + 1 * float4(g_positions[i].x, g_positions[i].y, 0, 0);

	
		Out.position = mul(position, World);
		float3 eyeDir = normalize(Out.position - EyePos.xyz);
		Out.texcoord2 = reflect(eyeDir, float3(0, 1, 0));

		Out.WPos = Out.position;
		Out.position = mul(Out.position, ViewProjection);
		Out.position = mul(Out.position, Projection);

		Out.color = In[0].color;
		Out.texcoord = (g_texcoords[i] + In[0].texcoord) % 2.0f;

		//float3 worldNormal = mul(float3(0,1,0), World);
		//float3 viewNormal = mul(float3(0, 1, 0), ViewProjection);
		//Out.texcoord2 = reflect(normalize(viewPosition), normalize(viewNormal));

		SpriteStream.Append(Out);
	}

	SpriteStream.RestartStrip();
}

float FrenselEquations(float reflectionCoef, float3 H, float3 V) {
	return (reflectionCoef + (1.0f - reflectionCoef) * pow(1.0 - saturate(dot(V, H)), 5.0));
}

float3 ReflectionFrensel(float4 posw, float4 norw, float4 eye, float eta)
{
	//float3 N = norw;
	/*float3 I = normalize(posw.xyz - eye);
	float3 R = reflect(I, N);
	float3 T = refract(I, N, eta);
	float fresnel = FrenselEquations(pow(eta - 1 / eta + 1, 2), N, I);

	float3 reflecColor = EnviromentMap.Sample(g_SamplerLinear, R);
	float3 refracColor = EnviromentMap.Sample(g_SamplerLinear, T);

	float3 col = lerp(refracColor, reflecColor, fresnel);*/

	return float3(1,1,1);
}

float4 GroundPS(GSParticleOut In) : SV_Target
{
	//float4 texcol = g_Tex.Sample(g_SamplerLinear,  In.texcoord);
	float4 texcol2 = SkyMap.Sample(g_SamplerLinear, In.texcoord2);
	float4 col= texcol2;
	//col = texcol * In.color * texcol2;


	//col.xyzw = saturate(col.xyzw * pow(2, 255 * (col.w - 0.5)));

	/*float4 fogColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
	col = In.fogFactor * col + (1.0 - In.fogFactor) * fogColor;*/

	return col;
}

// 視点はインクルードする
