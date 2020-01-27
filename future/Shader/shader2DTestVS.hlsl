cbuffer ConstantBuffer:register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
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
	outPosition = mul(inPosition,World);
	outPosition = mul(outPosition,Projection);
	outTexCoord = inTexCoord;
	outDiffuse = inDiffuse;
}