#include	"psvscommon.hlsl"

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT main( float4 Pos : POSITION,
			    float4 Color : COLOR,
				float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	// �X�N���[�����W�v�Z�p
	//float4 ScreenPos;

	// ���W�ϊ�
	//Pos.w = 1.0f;
	//ScreenPos = mul(Pos, World);			// ���W�ϊ�

	// �X�N���[�����W�ɕϊ�
	//output.Position = ScreenPos;
	//output.Position.x = (ScreenPos.x / ViewportWidth.x) * 2 - 1.0f;
	//output.Position.y = 1.0f - (ScreenPos.y / ViewportHeight.x) * 2;

	//output.Position.z = Pos.z;				
	//output.Position.w = 1.0f;
    
    output.Position = mul(Pos, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    
	// �J���[�l�Z�b�g
	output.Color = Color;
	// �e�N�X�`�����W�Z�b�g
	output.Tex = Tex;
	return output;
}