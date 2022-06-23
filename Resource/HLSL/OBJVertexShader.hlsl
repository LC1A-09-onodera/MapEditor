#include "OBJHeader.hlsli"

VSOutput main( float4 pos : POSITION , float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	//法線にワールド行列のスケーリング等を適用
	float4 wnormal = normalize(mul(world, float4(normal, 0.0f)));
	float4 wpos = mul(world, pos);
	
	//法線にワールド行列のスケーリング等を適用
    float4 rWnormal = normalize(mul(rWorld, float4(normal, 0.0f)));
    float4 rWpos = mul(rWorld, pos);

	VSOutput output;
	//output.svpos = mul(mat,pos);
	output.svpos = mul(mul(viewproj, world), pos);
	output.worldpos = wpos;
	output.normal = wnormal.xyz;
	output.uv = uv;
	
    //output.rSvpos = mul(mul(rViewproj, rWorld), pos);
    //output.rWorldpos = rWpos;
    //output.rNormal = rWnormal.xyz;
    //output.rUv = uv;
	return output;
}