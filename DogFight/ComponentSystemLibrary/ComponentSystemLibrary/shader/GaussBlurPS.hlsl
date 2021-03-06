//-------------------------------------------------------------------------------------------------
// File : GaussBlur.hlsl
// Desc : Gauss Blur.
// Copyright(c) Project Asura. All right reserved.
//-------------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////////////////////////
// VSOutput structure
///////////////////////////////////////////////////////////////////////////////////////////////////
struct VSOutput
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// CBuffer structure
///////////////////////////////////////////////////////////////////////////////////////////////////
cbuffer CbBlur: register(b3)
{
    int     SampleCount;
    float4  Offset[16];
};

//-------------------------------------------------------------------------------------------------
// Textures and Samplers.
//-------------------------------------------------------------------------------------------------
Texture2D       ColorBuffer  : register(t0);
SamplerState    ColorSampler : register(s0);

//-------------------------------------------------------------------------------------------------
//      メインエントリーポイントです.
//-------------------------------------------------------------------------------------------------
float4 main(const VSOutput input) : SV_TARGET0
{
    float4 result = 0;
    
    //result.w = 1.0f;
    for(int i=0; i<SampleCount; ++i)
    { result += Offset[i].z * ColorBuffer.Sample(ColorSampler, input.TexCoord + Offset[i].xy); }

    

    return result;
}