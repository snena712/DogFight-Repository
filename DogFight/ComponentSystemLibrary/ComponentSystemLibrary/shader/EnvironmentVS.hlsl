//--------------------------------------------------------------------------------------
// vsstage.fx
//--------------------------------------------------------------------------------------
#include	"psvscommonOther.hlsl"

struct VSOUT
{
	float4 Pos : SV_Position;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	float3 texcoord2: TEXCOORD1;
	float fogFactor : FOG;
};

TextureCube<float4>	SkyMap: register(t1);

//--------------------------------------------------------------------------------------
// 頂点シェーダー
//--------------------------------------------------------------------------------------
VSOUT main(float4 Pos : POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD)
{
	VSOUT output = (VSOUT)0;

	output.Pos = mul(Pos, World);
	
	

	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	
	float3 eyeDir = normalize(Pos.xyz - EyePos.xyz);
	output.texcoord2 = reflect(eyeDir, Normal.xyz);	
	output.texcoord = Tex;
	

	return output;
}