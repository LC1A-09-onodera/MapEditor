#include "Sample.hlsli"
Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);
#define  Iterations  8

float4 main(VSOutput input) : SV_TARGET
{
	//水面ぽいの
	float2 windowSize;
	windowSize.x = 1280;
	windowSize.y = 720;
	float speed = 1.0f;
	float ripple = 1.0f;
	float2 uv = input.uv;
	uint frameTime= frameCount;
	float time = frameTime * 0.01;
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
	//return tex0.Sample(smp, uv) + col * 0.1f;

	//テクスチャマッピング
	float4 texcolor = tex.Sample(smp, uv) + col * 0.1f;
	//シェーディングによる色
	float4 shadecolor;
	//光沢度
	const float shininess = 4.0f;
	//頂点から視点への方向ベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	//ライトに向かうベクトルと法線のベクトル
	float3 dotlightnormal = dot(lightv, input.normal);
	//反射光ベクトル
	float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	//環境光
	float3 ambient = m_ambient;
	float3 diffuse = dotlightnormal * m_diffuse;
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	shadecolor.rgb = (ambient + diffuse + specular) * lightcolor;
	shadecolor.a = m_alpha;
	for (int i = 0; i < POINTLIGHT_NUM; i++)
	{
		if (pointLights[i].active)
		{
			//ライトベクトル
			float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
			//ベクトルの長さ
			float d = length(lightv);
			//正規化
			lightv = normalize(lightv);
			//距離減衰係数
			float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);
			//内積値
			float3 dotlightnormal = dot(lightv, input.normal);
			//反射光
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;
			//鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//すべてを加算
			shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
		}
	}
	for (int i = 0; i < SPOTLIGHT_NUM; i++)
	{
		if (spotLights[i].active)
		{
			//ライトの方向ベクトル
			float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);
			//距離減衰係数
			float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));
			//角度係数
			float cos = dot(lightv, spotLights[i].lightv);
			//減衰開始の内側は1倍、それより外は0倍
			float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
			//減衰を乗算
			atten *= angleatten;
			//ライトに向かうベクトルと法線の内積
			float3 dotlightnormal = dot(lightv, input.normal);
			//反射光ベクトル
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//拡散反射光
			float3 diffuse = dotlightnormal * m_diffuse;
			//鏡面反射光
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//全ての加算
			shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
		}
	}
	for (int i = 0; i < CIRCLESHADOW_NUM; i++)
	{
		if (circleShadows[i].active)
		{
			float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			float d = dot(casterv, circleShadows[i].dir);
			float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
			atten *= step(0, d);
			float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			float3 lightv = normalize(lightpos - input.worldpos.xyz);
			float cos = dot(lightv, circleShadows[i].dir);
			float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			atten *= angleatten;
			shadecolor.rgb -= atten;
		}
	}
	float4 result;
	//texcolor.a = 0.5f;
	result = shadecolor * texcolor;
    result.x += 0.3f;
    result.y += 0.3f;
    result.z += 0.4f;
    result.a = 0.8f;
    return result;
}