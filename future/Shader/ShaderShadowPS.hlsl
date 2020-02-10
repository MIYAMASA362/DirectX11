#include"Constant.hlsli"
Texture2D	 Texture[3]:register(t0);
SamplerState Sampler:register(s0);


cbuffer LightBuffer:register(b1)
{
	LIGHT Light;
}

cbuffer CameraBuffer : register(b2)
{
	CAMERA Camera;
}

void main(
	in float4 inPosition			: SV_POSITION,
	in float4 inWorldPosition : POSITION0,
	in float4 inNormal : NORMAL0,
	in float4 inBinormal : BINORMAL0,
	in float4 inTangent : TANGENT0,
	in float4 inDiffuse : COLOR0,
	in float2 inTexCoord : TEXCOORD0,
	out float4 outDiffuse : SV_Target
)
{
	//視点ベクトル
	float3 eyev = inWorldPosition.xyz - Camera.Position.xyz;

	//視差マップ
	eyev = normalize(eyev);
	matrix mat = { inBinormal,-inTangent,inNormal,float4(0,0,0,1) };
	matrix invmat = transpose(mat);
	float3 invEyev = mul(eyev, invmat).xyz;

	float heightMap = (Texture[1].Sample(Sampler, inTexCoord).r - 0.5) * 0.2;
	inTexCoord += invEyev.xy * heightMap;

	inNormal = normalize(inNormal);

	//法線マップ
	float4 normalMap = Texture[0].Sample(Sampler, inTexCoord);
	normalMap = (normalMap * 2.0) - 1;

	normalMap = (normalMap.x * inBinormal) + (-normalMap.y * inTangent) + (normalMap.z * inNormal);
	normalMap.w = 0.0;
	normalMap = normalize(normalMap);

	//スペキュラ
	float3 refv = reflect(Light.Direction.xyz - inWorldPosition.xyz, normalMap.xyz);
	refv = normalize(refv);

	float specular = -dot(eyev, refv);
	specular = saturate(specular);
	specular = pow(specular, 10);

	//ライティング
	float light = -dot(normalMap.xyz, normalize(Light.Direction.xyz));
	light = saturate(light);
	outDiffuse = Texture[2].Sample(Sampler, inTexCoord) * light;
	outDiffuse += specular;
	outDiffuse.a = 1.0;
}