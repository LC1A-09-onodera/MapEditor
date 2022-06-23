#include "Particle.hlsli"

static const uint vnum = 4;

static const float4 offset_array[vnum] = {
	float4(-0.5f, -0.5f, 0, 0),
	float4(-0.5f, +0.5f, 0, 0),
	float4(+0.5f, -0.5f, 0, 0),
	float4(+0.5f, +0.5f, 0, 0),
};

static const float2 uv_array[vnum] = 
{
	float2(0, 1),
	float2(0, 0), 
	float2(1, 1),
	float2(1, 0),
};

float3 rotate(float3 position, float angle, float3 axis)
{
	float3 a = normalize(axis);
	float s = sin(angle);
	float c = cos(angle);
	float r = 1.0 - c;
	float3x3 m = float3x3(
		a.x * a.x * r + c, a.y * a.x * r + a.z * s, a.z * a.x * r - a.y * s,
		a.x * a.y * r - a.z * s, a.y * a.y * r + c, a.z * a.y * r + a.x * s,
		a.x * a.z * r + a.y * s, a.y * a.z * r - a.x * s, a.z * a.z * r + c
		);

	return mul(m, position);
}

//ƒ‰ƒ“ƒ_ƒ€‚È’l‚ð•Ô‚·
float rand(float2 co)
{
	return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453);
}

[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{	
	GSOutput element;
	for (uint i = 0; i < vnum; i++)
	{
		float4 offset = offset_array[i] * input[0].scale;
		
		offset = mul(matBillboard, offset);
		element.svpos = input[0].pos + offset;
		element.svpos = mul(mat, element.svpos);
		element.uv = uv_array[i];
		output.Append(element);
	}
}