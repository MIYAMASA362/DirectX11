#include"Module\Module.h"

const float DirectX::Physics::m_Gravity = -0.98f;

Vector3 DirectX::Physics::Get_Gravity()
{
	return Vector3::up() * Physics::m_Gravity;
}

