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

	float3 refv = reflect(Light.Direction.xyz - inWorldPosition.xyz,inNormal.xyz);
	refv = normalize(refv);

	float specular = -dot(eye,refv);
	specular = saturate(specular);
	specular = pow(specular,10);

	//ハーフランバート　ライトベクトルとの内積
	float light = 0.5 - dot(inNormal.xyz,float3(0.0,-1.0,0.0))*0.5;

	//-N・Eの計算
	float facing = -dot(inNormal.xyz, eye);
	float2 toonTexCoord;
	toonTexCoord.x = light;
	toonTexCoord.y = facing;

	//下記エラーが出たら outDiffuse = Texture.Samp----;
	outDiffuse.rgb = Texture[0].Sample(Sampler, toonTexCoord);
	outDiffuse.a = 1.0;

	outDiffuse.rgb *= Texture[1].Sample(Sampler,inTexCoord);
	outDiffuse.rgb += specular;
}