#include"Constant.hlsli"

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

cbuffer CameraBuffer : register(b2)
{
	CAMERA Camera;
}



void main(
	in float4 inPosition		: POSITION0,
	in float4 inNormal : NORMAL0,
	in float4 inBinormal : BINORMAL0,
	in float4 inTangent : TANGENT0,
	in float4 inDiffuse : COLOR0,
	in float2 inTexCoord : TEXCOORD0,

	out float4 outPosition : SV_POSITION,
	out float4 outWorldPosition : POSITION0,
	out float4 outNormal : NORMAL0,
	out float4 outBinormal : BINORMAL0,
	out float4 outTangent : TANGENT0,
	out float4 outDiffuse : COLOR0,
	out float2 outTexCoord : TEXCOORD0,
	out float4 outShadowCoord : TEXCOORD1
)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	outPosition = mul(inPosition, wvp);
	outWorldPosition = mul(inPosition, World);

	inNormal.w = 0.0;
	outNormal = mul(inNormal, World);

	inBinormal.w = 0.0;
	outBinormal = mul(inBinormal, World);

	inTangent.w = 0.0;
	outTangent = mul(inTangent, World);

	outDiffuse = inDiffuse;
	outTexCoord = inTexCoord;

	matrix lightwvp;
	lightwvp = mul(World,Light.View);
	lightwvp = mul(lightwvp,Light.Projection);
	outShadowCoord = mul(inPosition,lightwvp);
}