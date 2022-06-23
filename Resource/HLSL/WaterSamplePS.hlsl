#include "OBJHeader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

#define  Iterations  8

float4 main(VSOutput input) : SV_TARGET
{
    //水面ぽいの
    float2 windowSize;
    windowSize.x = 1280;
    windowSize.y = 720;
    float speed = 0.8f;
    float ripple = 1.2f;
    float2 uv = input.uv;
    uint frameCount = flash;
    float time = frameCount * 0.01;
    float2 pos = input.svpos.xy / windowSize * 12.0 - 20.0;
    float2 tmp = pos;
    float speed2 = speed * 2.0;
    float inten = 0.015f;
    float col = 0;
    float distortion = 0.2f;
    for (int i = 0; i < Iterations; ++i)
    {
        float t = time * (1.0 - (3.2 / (float(i) + speed)));
        tmp = pos + float2(
		cos(t - tmp.x * ripple) + sin(t + tmp.y * ripple),
		sin(t - tmp.y * ripple) + cos(t + tmp.x * ripple));
        tmp += time;
        col += 1.0 / length(float2(pos.x / (sin(tmp.x + t * speed2) / inten), pos.y / (cos(tmp.y + t * speed2) / inten)));
    }
    col /= float(Iterations);
    col = saturate(1.5 - sqrt(col));
    uv += col * distortion;
    return tex.Sample(smp, uv) + col * 0.1f;
	//テクスチャマッピング
    //float4 texcolor = tex.Sample(smp, input.uv);
    //texcolor.x += 0.4f;
    //return texcolor;
}