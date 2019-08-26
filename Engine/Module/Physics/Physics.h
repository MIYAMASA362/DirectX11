#pragma once

namespace DirectX
{
	class Physics
	{
	public:
		static const float m_Gravity;
	public:
		Physics() = delete;
		~Physics() = delete;
	public:
		static Vector3 Get_Gravity();
	};
}