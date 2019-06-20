#pragma once

#include"DirectXStruct.h"

namespace DirectX
{
	//•ÏŠ·
	class Transform
	{
	public:
		Vector3 position;
		Vector3 scale;
		Vector3 rotation;
	public:
		Transform(Vector3 position, Vector3 scale, Vector3 rotation);
		Transform();
	public:
		Vector3 forward();
		Vector3 back();
		Vector3 left();
		Vector3 right();
		Vector3 up();
		Vector3 down();
	public:
		XMMATRIX MatrixRotationRollPitchYaw();
		XMMATRIX MatrixTranslation();
		XMMATRIX MatrixScaling();
		XMMATRIX WorldMatrix();
	};
}