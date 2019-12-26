#pragma once

class Rigidbody;
class MeshRender;

struct RayCastHit
{
	Vector3 hitPosition;
	float distance;
	Vector3 normal;
};

class Physics
{
private:
	static std::vector<std::weak_ptr<MeshRender>> _RayCastTarget;	//RayCast”»’è‚ÉŽg‚¤Mesh
public:
	static const float m_Gravity;
	static Vector3 Get_Gravity();
	static bool RayCast(Vector3 origin,Vector3 direction,RayCastHit& info,float distance);
	static void AddRayCastTarget(std::weak_ptr<MeshRender> meshRender);
};