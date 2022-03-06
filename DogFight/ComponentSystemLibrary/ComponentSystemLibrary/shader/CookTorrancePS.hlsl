#include	"psvscommonOther.hlsl"



//--------------------------------------------------------------------------------------
//  �s�N�Z���V�F�[�_�[
//--------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//      Schlick�ɂ��t���l�����̋ߎ���.
//-----------------------------------------------------------------------------
float3 SchlickFresnel(float3 specular, float VH)
{
	return specular + (1.0f - specular) * pow((1.0f - VH), 5.0f);
}

//-----------------------------------------------------------------------------
//      Beckmann���z�֐�.
//-----------------------------------------------------------------------------
float D_Beckmann(float m, float NH)
{
	float c2 = NH * NH;
	float c4 = c2 * c2;
	float m2 = m * m;

	return (1.0f / (m2 * c4)) * exp((-1.0f / m2) * (1.0f - c2) / c2);
}

//-----------------------------------------------------------------------------
//      V-cavity�ɂ��V���h�E�C���O-�}�X�L���O�֐�.
//-----------------------------------------------------------------------------
float G2_Vcavity(float NH, float NV, float NL, float VH)
{
	return min(1.0f, min(2.0f * NH * NV / VH, 2.0f * NH * NL / VH));
}

static const float F_PI = 3.141596535f;
float4 main(VS_OUTPUT input) : SV_Target
{
	 float4 LightColor = float4(2, 2, 2, 1);
	 const float Roughness = 0.5f;  // �e��
	 const float Metallic = 0.5f;   // �����x

	 float3 n = input.Normal.xyz;
	 float3 N = normalize(n);
	 float3 L = normalize(LightDirection);				// ���C�g�x�N�g��  koko	
	 float3 V = normalize(EyePos - input.WPos);			// ���_�x�N�g��
	 float3 R = normalize(-V + 2.0f * dot(N, V) * N);	// 
	 float3 H = normalize(V + L);						// ���C�g�Ǝ��_�x�N�g���̃n�[�t�x�N�g��

	 float NH = saturate(dot(N, H));					// ���C�g�Ǝ��_�x�N�g���̃n�[�t�x�N�g���Ɩ@���Ƃ̓��ς��v�Z
	 float NV = saturate(dot(N, V));					// ���[���h��ԏ�̎��_�i�J�����j�ʒu�Ɩ@���Ƃ̓��ς��v�Z
	 float NL = saturate(dot(N, L));					// ���[���h��ԏ�̃��C�g�ʒu�Ɩ@���Ƃ̓��ς��v�Z
	 float VH = saturate(dot(V, H));					// ���_�x�N�g���ƃ��C�g�Ǝ��_�x�N�g���̃n�[�t�x�N�g���Ƃ̓��ς��v�Z

	 float4 texcolor = g_Tex.Sample(g_SamplerLinear, input.Tex);	// �e�N�X�`��
	 float3 basecolor = float3(1, 1, 1);							// ��{�F

	 float3 Kd = basecolor * (1.0f - Metallic);
	 float3 diffuse = Kd * (1.0 / F_PI);				// ���̊g�U����

	 float3 Ks = basecolor * Metallic;
	 float  a = Roughness * Roughness;
	 float  D = D_Beckmann(a, NH);
	 float  G2 = G2_Vcavity(NH, NV, NL, VH);
	 float3 Fr = SchlickFresnel(Ks, NL);

	 float3 specular = (D * G2 * Fr) / (4.0f * NV * NL);

	 float4  output = float4(LightColor * texcolor.rgb * (diffuse + specular)* NL, 1);
	 return output;
}
