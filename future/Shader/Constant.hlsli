struct LIGHT
{
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
	float4 Position;
	matrix View;
	matrix Projection;
};

struct CAMERA
{
	float4 Position;
};