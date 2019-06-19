#pragma once

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
		Transform(Vector3 position, Vector3 scale, Vector3 rotation) :
			position(position),
			scale(scale),
			rotation(rotation)
		{};
		Transform():Transform(Vector3::zero(),Vector3::one(),Vector3::zero()){};
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