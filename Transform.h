#pragma once

namespace DirectX
{
	//�ϊ�
	class Transform
	{
	public:
		Vector3 position;
		Vector3 scale;
		Vector3 rotation;
	public:
		Transform() :
			position(0.0f, 0.0f, 0.0f),
			scale(1.0f, 1.0f, 1.0f), 
			rotation(0.0f, 0.0f, 0.0f) 
		{};
	public:
		Vector3 forward()	{ return Vector3::forward();};
		Vector3 back()		{ return Vector3::back();	};
		Vector3 left()		{ return Vector3::left();	};
		Vector3 right()		{ return Vector3::right();	};
		Vector3 up()		{ return Vector3::up();		};
		Vector3 down()		{ return Vector3::down();	};
	public:
		XMMATRIX MatrixRotationRollPitchYaw() 
			{ return XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z); };
		XMMATRIX MatrixTranslation()			
			{ return XMMatrixTranslation(position.x, position.y, position.z); };
		XMMATRIX MatrixScaling()				
			{ return XMMatrixScaling(scale.x, scale.y, scale.z); };
		XMMATRIX WorldMatrix()					
			{ return MatrixScaling() * MatrixRotationRollPitchYaw() * MatrixTranslation(); };
	};
}