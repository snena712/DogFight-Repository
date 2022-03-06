#include	"psvscommon.hlsl"


//cbuffer FogBuffer
//{
//	float fogStart;
//	float fogEnd;
//};


struct VSOut
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float fogFactor : FOG;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
VSOut VS(float4 Pos : POSITION,
	float4 Normal : NORMAL,
	float2 Tex : TEXCOORD)
{
	VSOut output;
	float4 cameraPosition;


	// Change the position vector to be 4 units for proper matrix calculations.
	Pos.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(Pos, World);
	output.position = mul(Pos, View);
	output.position = mul(Pos, Projection);

	// Store the texture coordinates for the pixel shader.
	output.tex = Tex;

	// Calculate the camera position.
	cameraPosition = mul(Pos, World);
	cameraPosition = mul(cameraPosition, View);


	float fogStart = 10;	
	float fogEnd = 1000;

	// Calculate linear fog.    
	output.fogFactor = saturate((fogEnd - cameraPosition.z) / (fogEnd - fogStart));

	return output;
}

float4 PS(VSOut input) : SV_TARGET
{
	float4 textureColor;
	float4 fogColor;
	float4 finalColor;


	// Sample the texture pixel at this location.
	textureColor = g_Tex.Sample(g_SamplerLinear, input.tex);

	// Set the color of the fog to grey.
	fogColor = float4(0.5f, 0.5f, 0.5f, 1.0f);

	// Calculate the final color using the fog effect equation.
	finalColor = input.fogFactor * textureColor + (1.0 - input.fogFactor) * fogColor;

	return finalColor;
}
