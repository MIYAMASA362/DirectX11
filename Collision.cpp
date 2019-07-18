#include "Collision.h"

void DirectX::Collider::OnTrigger(std::weak_ptr<Collider> trigger1, std::weak_ptr<Collider> trigger2)
{
	//AABB
	auto AABBoxVsAABBox = [](BoxCollier* box1,BoxCollier* box2)
	{
		Vector3 Box1Pos = box1->transform.lock()->position() + box1->center;
		Vector3 Box2Pos = box2->transform.lock()->position() + box2->center;

		if (
			Box1Pos.x + box1->scale.x >= Box2Pos.x - box2->scale.x && Box1Pos.x - box1->scale.x <= Box2Pos.x + box2->scale.x &&
			Box1Pos.y + box1->scale.y >= Box2Pos.y - box2->scale.y && Box1Pos.y - box1->scale.y <= Box2Pos.y + box2->scale.y &&
			Box1Pos.z + box1->scale.z >= Box2Pos.z - box2->scale.z && Box1Pos.z - box1->scale.z <= Box2Pos.z + box2->scale.z
			) {
			return true;
		}

		return false;
	};

	auto SphereVsSphere = [](SphereCollider* sphere1,SphereCollider* sphere2)
	{
		Vector3 Sphere1Pos = sphere1->transform.lock()->position() + sphere1->center;
		Vector3 Sphere2Pos = sphere2->transform.lock()->position() + sphere2->center;

		Vector3 vec = Sphere2Pos - Sphere1Pos;
		float lengthsq = vec.lengthSq();

		if (sphere1->raidus * sphere1->raidus + sphere2->raidus * sphere2->raidus < lengthsq) return false;

		//‚ß‚è‚İ—Ê
		float length = (sphere1->raidus + sphere2->raidus) - sqrtf(lengthsq);
		//‚ß‚è‚İƒxƒNƒgƒ‹
		vec = vec.normalize() * length;

		return true;
	};
}