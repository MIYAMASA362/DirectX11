#include"Common.h"
#include<memory>

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"../ECSEngine.h"

#include"Physics.h"
#include"Rigidbody.h"

using namespace DirectX;

const float DirectX::Physics::m_Gravity = -0.98f;

Vector3 DirectX::Physics::Get_Gravity()
{
	return Vector3::up() * Physics::m_Gravity;
}

bool DirectX::Physics::RayCast(Vector3 origin, Vector3 direction, float distance)
{
	return false;
}