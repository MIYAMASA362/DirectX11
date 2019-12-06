#pragma once

class Light
{
private:
	struct Constant
	{
		Vector4 Direction;
		Color Diffuse;
		Color Ambient;
	};
	Light::Constant _constant;
	ID3D11Buffer* _LightBuffer;
public:
	Light();
	virtual ~Light();
	
	void SetResource();
};