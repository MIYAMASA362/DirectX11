#pragma once

class Light
{
public:
	struct Constant
	{
		Vector4 Direction;
		Color Diffuse;
		Color Ambient;
	};
	Light::Constant _constant;
public:
	Light();
	virtual ~Light();

	void SetResource();
};