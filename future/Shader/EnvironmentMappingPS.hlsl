Texture2D	 Texture[2]:register(t0);
SamplerState Sampler:register(s0);

struct LIGHT
{
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
	float4 Position;
};

struct CAMERA
{
	float4 Position;
};



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
	in float4 inDiffuse : COLOR0,
	in float2 inTexCoord : TEXCOORD0,
	out float4 outDiffuse : SV_Target
)
{
	inNormal = normalize(inNormal);
	float3 eye = inWorldPosition.xyz - Camera.Position.xyz;
	eye = normalize(eye);

	//スペキュラ計算
	float3 refv = reflect(Light.Direction.xyz - inWorldPosition.xyz, inNormal.xyz);
	refv = normalize(refv);

	float specular = -dot(eye, refv);
	specular = saturate(specular);
	specular = pow(specular, 10);


	//環境マッピング
	float3 refvec = reflect(eye.xyz,inNormal);
	refvec = normalize(refvec);

	float2 envTexCoord;
	envTexCoord.x = -refvec.x * 0.3 + 0.5;
	envTexCoord.y = -refvec.y * 0.3 + 0.5;


	outDiffuse.rgb = Texture[0].Sample(Sampler, envTexCoord);
	outDiffuse.a = 1.0;

	outDiffuse.rgb *= Texture[1].Sample(Sampler, inTexCoord);
	outDiffuse.rgb += specular;
}