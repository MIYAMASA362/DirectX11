#pragma once

namespace DirectX
{
	class Rigidbody;
	class Physics
	{
	public:
		static const float m_Gravity;
		static Vector3 Get_Gravity();
		static bool RayCast(Vector3 origin,Vector3 direction,float distance);
	};
}