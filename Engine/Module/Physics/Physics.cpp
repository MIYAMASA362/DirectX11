#define INCLUDE_CEREAL
#include"Common.h"
#include<memory>
#include<algorithm>

#include"Module\DirectX\DirectX.h"

#include"../ECSEngine.h"

#include"Module\Transform\Transform.h"
#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"

#include"Module\Renderer\Renderer.h"
#include"Module\Mesh\Mesh.h"
#include"Module\Mesh\MeshRender.h"

#include"Physics.h"
#include"Rigidbody.h"

using namespace DirectX;

//*********************************************************************************************************************
//
//	Physics
//
//*********************************************************************************************************************

std::vector<std::weak_ptr<MeshRender>> Physics::_RayCastTarget;

const float Physics::m_Gravity = -0.98f;

//Get_Gravity
//	重力の取得
//
Vector3 Physics::Get_Gravity()
{
	return Vector3::up() * Physics::m_Gravity;
}

//RayCast
//	レイキャスト
//
bool Physics::RayCast(Vector3 origin, Vector3 direction,RayCastHit& info, float distance)
{
	//削除
	_RayCastTarget.erase(
		std::remove_if(_RayCastTarget.begin(), _RayCastTarget.end(), 
			[](std::weak_ptr<MeshRender> render) 
	{ 
		bool enable =  render.expired(); 
		return enable;
	}),		
		_RayCastTarget.end());
	
	direction = direction.normalize();

	//判定
	for (auto meshrender : _RayCastTarget)
	{
		//Meshから頂点取得
		{
			auto mesh = meshrender.lock()->GetMeshFilter()->GetMesh().lock();
			auto world = meshrender.lock()->gameObject()->transform().lock()->WorldMatrix();

			bool hit = false;
			for (unsigned int i = 0; i < mesh->_SurfaceNum; i++)
			{
				auto surface = mesh->_Surface[i];

				//三角頂点
				Vector3 p1;
				p1 = surface._p1;
				p1 = XMVector3TransformCoord(XMVectorSet(p1.x, p1.y, p1.z, 1.0f), world);
				Vector3 p2;
				p2 = surface._p2;
				p2 = XMVector3TransformCoord(XMVectorSet(p2.x, p2.y, p2.z, 1.0f), world);
				Vector3 p3;
				p3 = surface._p3;
				p3 = XMVector3TransformCoord(XMVectorSet(p3.x, p3.y, p3.z, 1.0f), world);

				//法線
				Vector3 normal = Vector3::Cross((p2 - p1), (p3 - p1));
				normal = normal.normalize();

				if (normal == Vector3::zero()) continue;

				//無限平面上計算
				float dnd = Vector3::Dot(direction, normal);

				float t = Vector3::Dot(normal, (p1 - origin)) / dnd;

				Vector3 pos = origin + direction * t;

				//三角形に内包されるか
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
	}
	return false;
}

void Physics::AddRayCastTarget(std::weak_ptr<MeshRender> meshRender)
{
	_RayCastTarget.push_back(meshRender.lock());
}
