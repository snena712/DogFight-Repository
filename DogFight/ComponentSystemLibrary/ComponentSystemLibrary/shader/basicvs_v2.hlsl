#include	"psvscommon_v2.hlsl"
#include	"Matrix.hlsl"
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT main(float4 Pos : POSITION,
	float4 Color : COLOR,
	float2 Tex : TEXCOORD,
	float4 QT : QUATER,
	float4 Wpos : WPOS)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	float3 _pos = float3(Wpos.x, Wpos.y, Wpos.z);
	float3 _scale = float3(1, 1, 2);
	float4x4 _mtx = compose(_pos, QT, _scale);
	//_mtx =inverse(_mtx);

	output.Position = mul(Pos, _mtx);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	// カラー値セット
	output.Color = Color;
	// テクスチャ座標セット
	output.Tex = Tex;
	return output;
}

//行列用の関数