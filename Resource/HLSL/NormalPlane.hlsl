#include "OBJHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

#define  Iterations  8

float4 main(VSOutput input) : SV_TARGET
{
    //テクスチャマッピング
    float4 texcolor = tex.Sample(smp, input.uv);
    //texcolor.x += 0.4f;
    return texcolor;
}
