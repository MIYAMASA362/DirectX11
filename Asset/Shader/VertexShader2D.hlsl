cbuffer WorldBuffer : register(b0)
{
	matrix World;
}

cbuffer ProjectionBuffer :register(b2)
{
	matrix Projection;
}

void main(
	in  float4 inPosition		: POSITION0,
	in  float4 inNormal			: NORMAL0,
	in  float4 inDiffuse		: COLOR0,
	in  float2 inTexCoord		: TEXCOORD0,

	out float4 outPosition		: SV_POSITION,
	out float4 outNormal		: NORMAL0,
	out float2 outTexCoord		: TEXCOORD0,
	out float4 outDiffuse		: COLOR0
)
{
	matrix wp;
	wp = mul(World,Projection);

	outPosition = mul(inPosition,wp);
	outNormal = inNormal;
	outTexCoord = inTexCoord;

	outDiffuse = inDiffuse;
}