#include	"psvscommonOther.hlsl"



//--------------------------------------------------------------------------------------
//  ピクセルシェーダー
//--------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//      Schlickによるフレネル項の近似式.
//-----------------------------------------------------------------------------
float3 SchlickFresnel(float3 specular, float VH)
{
	return specular + (1.0f - specular) * pow((1.0f - VH), 5.0f);
}

//-----------------------------------------------------------------------------
//      Beckmann分布関数.
//-----------------------------------------------------------------------------
float D_Beckmann(float m, float NH)
{
	float c2 = NH * NH;
	float c4 = c2 * c2;
	float m2 = m * m;

	return (1.0f / (m2 * c4)) * exp((-1.0f / m2) * (1.0f - c2) / c2);
}

//-----------------------------------------------------------------------------
//      V-cavityによるシャドウイング-マスキング関数.
//-----------------------------------------------------------------------------
float G2_Vcavity(float NH, float NV, float NL, float VH)
{
	return min(1.0f, min(2.0f * NH * NV / VH, 2.0f * NH * NL / VH));
}

static const float F_PI = 3.141596535f;
float4 main(VS_OUTPUT input) : SV_Target
{
	 float4 LightColor = float4(2, 2, 2, 1);
	 const float Roughness = 0.5f;  // 粗さ
	 const float Metallic = 0.5f;   // 金属度

	 float3 n = input.Normal.xyz;
	 float3 N = normalize(n);
	 float3 L = normalize(LightDirection);				// ライトベクトル  koko	
	 float3 V = normalize(EyePos - input.WPos);			// 視点ベクトル
	 float3 R = normalize(-V + 2.0f * dot(N, V) * N);	// 
	 float3 H = normalize(V + L);						// ライトと視点ベクトルのハーフベクトル

	 float NH = saturate(dot(N, H));					// ライトと視点ベクトルのハーフベクトルと法線との内積を計算
	 float NV = saturate(dot(N, V));					// ワールド空間上の視点（カメラ）位置と法線との内積を計算
	 float NL = saturate(dot(N, L));					// ワールド空間上のライト位置と法線との内積を計算
	 float VH = saturate(dot(V, H));					// 視点ベクトルとライトと視点ベクトルのハーフベクトルとの内積を計算

	 float4 texcolor = g_Tex.Sample(g_SamplerLinear, input.Tex);	// テクスチャ
	 float3 basecolor = float3(1, 1, 1);							// 基本色

	 float3 Kd = basecolor * (1.0f - Metallic);
	 float3 diffuse = Kd * (1.0 / F_PI);				// 光の拡散反射

	 float3 Ks = basecolor * Metallic;
	 float  a = Roughness * Roughness;
	 float  D = D_Beckmann(a, NH);
	 float  G2 = G2_Vcavity(NH, NV, NL, VH);
	 float3 Fr = SchlickFresnel(Ks, NL);

	 float3 specular = (D * G2 * Fr) / (4.0f * NV * NL);

	 float4  output = float4(LightColor * texcolor.rgb * (diffuse + specular)* NL, 1);
	 return output;
}
