//--------------------------------------------------------------------------------------
// ps.fx
//--------------------------------------------------------------------------------------
#include	"psvscommonOther.hlsl"

TextureCube<float4>	SkyMap: register(t1);


struct VSOUT
{
	float4 Pos : SV_Position;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	float3 texcoord2: TEXCOORD1;
	float fogFactor : FOG;
};

//--------------------------------------------------------------------------------------
//  ピクセルシェーダー
//--------------------------------------------------------------------------------------
float4 main(VSOUT input) : SV_Target
{
	 float4 texcol = g_Tex.Sample(g_SamplerLinear, input.texcoord);
	 float3 texcol2 = SkyMap.Sample(g_SamplerLinear, input.texcoord2).xyz;
	 float4 col = texcol;

	 // 輝度の取得
	float luminance = dot(texcol2, float3(0.215f, 0.7154f, 0.0721f));
		
	col.xyz += luminance - 0.6f;
	float4 C = float4(col.xyz,1);
	return C;
}