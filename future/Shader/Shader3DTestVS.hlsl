struct LIGHT
{
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
};

cbuffer ConstantBuffer:register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

cbuffer LightBuffer:register(b1)
{
	LIGHT Light;
}

void main(
	in float4 inPosition		:POSITION0,
	in float4 inNormal		:NORMAL0,
	in float4 inDiffuse		:COLOR0,
	in float2 inTexCoord		:TEXCOORD0,

	out float4 outPosition	:SV_POSITION,
	out float4 outDiffuse	:COLOR0,
	out float2 outTexCoord	:TEXCOORD0
)
{
	matrix wvp;
	wvp = mul(World,View);
	wvp = mul(wvp, Projection);
	outPosition = mul(inPosition,wvp);

	outTexCoord = inTexCoord;

	inNormal.w = 0.0;
	float4 worldNormal = mul(inNormal,World);
	worldNormal = normalize(worldNormal);

	float light = -dot(worldNormal,Light.Direction);
	light = saturate(light);

	outDiffuse = inDiffuse * (Light.Diffuse * light + Light.Ambient *(1.0 - light));
}