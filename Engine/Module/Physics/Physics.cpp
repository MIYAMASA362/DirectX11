#include"Common.h"
#include<memory>
#include<algorithm>

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"../ECSEngine.h"

#include"Module\Module.h"

#include"Physics.h"
#include"Rigidbody.h"

using namespace DirectX;

std::vector<std::weak_ptr<MeshRender>> Physics::_RayCastTarget;

const float DirectX::Physics::m_Gravity = -0.98f;

Vector3 DirectX::Physics::Get_Gravity()
{
	return Vector3::up() * Physics::m_Gravity;
}

bool DirectX::Physics::RayCast(Vector3 origin, Vector3 direction,RayCastHit& info, float distance)
{
	//ν
	_RayCastTarget.erase(
		std::remove_if(_RayCastTarget.begin(), _RayCastTarget.end(), 
			[](std::weak_ptr<MeshRender> render) 
	{ 
		bool enable =  render.expired(); 
		return enable;
	}),		
		_RayCastTarget.end());
	
	direction = direction.normalize();

	//»θ
	for (auto meshrender : _RayCastTarget)
	{
		//Mesh©ηΈ_ζΎ
		auto mesh = meshrender.lock()->mesh;
		auto world = meshrender.lock()->transform()->WorldMatrix();


		bool hit = false;
		for (int i = 0; i < mesh->_SurfaceNum; i++)
		{
			auto surface = mesh->_Surface[i];

			//OpΈ_
			Vector3 p1;
			p1 = surface._p1;
			p1 = XMVector3TransformCoord(XMVectorSet(p1.x, p1.y, p1.z, 1.0f), world);
			Vector3 p2;
			p2 = surface._p2;
			p2 = XMVector3TransformCoord(XMVectorSet(p2.x, p2.y, p2.z, 1.0f), world);
			Vector3 p3;
			p3 = surface._p3;
			p3 = XMVector3TransformCoord(XMVectorSet(p3.x, p3.y, p3.z, 1.0f), world);

			//@ό
			Vector3 normal = Vector3::Cross((p2 - p1), (p3 - p1));
			normal = normal.normalize();

			if (normal == Vector3::zero()) continue;

			float dnd = Vector3::Dot(direction, normal);
			float dno = Vector3::Dot(origin, normal);

			float t = Vector3::Dot(normal, (p1 - origin)) / dnd;

			Vector3 pos = origin + direction * t;

			//Op`Ιΰο³κι©
			Vector3 vp1 = Vector3::Cross((p1 - pos), (p2 - p1));
			vp1 = vp1.normalize();
			Vector3 vp2 = Vector3::Cross((p2 - pos), (p3 - p2));
			vp2 = vp2.normalize();
			Vector3 vp3 = Vector3::Cross((p3 - pos), (p1 - p3));
			vp3 = vp3.normalize();

			float vp1n = Vector3::Dot(vp1, normal);
			float vp2n = Vector3::Dot(vp2, normal);
			float vp3n = Vector3::Dot(vp3, normal);

			if (vp1n >= 0.0f && vp2n >= 0.0f && vp3n >= 0.0f)
			{
				if (t < info.distance || !hit)
				{
					info.distance = t;
					info.hitPosition = pos;
					info.normal = normal;
				}
				hit = true;
			}
		}
		if (hit) return true;
	}
	return false;
}

void DirectX::Physics::AddRayCastTarget(std::weak_ptr<MeshRender> meshRender)
{
	_RayCastTarget.push_back(meshRender.lock());
}
