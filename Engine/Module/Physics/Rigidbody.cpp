#include"Common.h"

#include"Module\DirectX\DirectX.h"

#include"Module\ECSEngine.h"

#include"Module\Physics\Collision.h"

#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"
#include"Module\Transform\Transform.h"

#include"Physics.h"
#include"Collision.h"
#include"Rigidbody.h"

using namespace DirectX;

Rigidbody::Rigidbody(EntityID OwnerID)
:
	Component(OwnerID),
	m_useGravity(true),
	m_velocity(Vector3::zero()),
	m_mass(1.0f)
{
	this->SendComponentMessage = [this](std::string message)
	{
		if (message == "FixedUpdate") FixedUpdate();
	};
	
	this->OnDebugImGui = [this]() 
	{
		if (ImGui::TreeNode("Rigidbody")) 
		{
			ImGui::Checkbox("UseGravity:",&this->m_useGravity);
			ImGui::InputFloat("Mass:",&this->m_mass);
			ImGui::InputFloat3("Velocity:",&this->m_velocity.x);
			ImGui::InputFloat3("OldPosition",&this->m_oldPosition.x);
			ImGui::TreePop();
		}
	};
}

Rigidbody::~Rigidbody()
{

}

void Rigidbody::FixedUpdate()
{
	Transform* m_transform = this->transform().get();

	//d—Í
	if (m_useGravity)
		m_velocity += m_mass * Physics::Get_Gravity();
	else
		m_velocity = Vector3::zero();

	//y:0‚ð’n–Ê‚Æ‚µ‚½”»’è
	/*if (m_transform->position().y - (m_transform->scale().y*0.5f) + m_velocity.y < 0.0f)
		m_velocity.y -= m_transform->position().y- (m_transform->scale().y* 0.5f) + m_velocity.y;*/
}

void Rigidbody::ApplyRigidbody()
{
	auto index = ComponentManager::GetOrCreateComponentIndex(Rigidbody::TypeID);
	for (auto component : *index.lock()) {
		auto rigidbody = std::dynamic_pointer_cast<Rigidbody>(component.lock());
		auto transform = rigidbody->transform();
		rigidbody->m_oldPosition = transform->position();
		transform->position(transform->position() + rigidbody->m_velocity);
	}
}

void DirectX::Rigidbody::CollisionRigidbody()
{
	auto index = ComponentManager::GetComponentIndex(Rigidbody::TypeID);

	std::shared_ptr<Rigidbody> rigidbody_1;

	for(auto component_1 : *index.lock())
	{
		rigidbody_1 = std::dynamic_pointer_cast<Rigidbody>(component_1.lock());
		Collider::Hitjudgment(rigidbody_1->_Colliders);
	}
}

Vector3 DirectX::Rigidbody::GetOldPosition()
{
	return this->m_oldPosition;
}

void DirectX::Rigidbody::OnDestroy()
{
	//auto rigidbody = this->transform()->GetComponentInParent(Rigidbody::TypeID);

	//if(rigidbody.expired())
		

	/*Collider* collider = this->gameObject()->GetComponent<Collider>();
	if(collider)
		collider->IsAttachRigdbody = false;*/
}

void Rigidbody::IsUseGravity(bool enable) 
{
	this->m_useGravity = enable;
}

void Rigidbody::SetMass(float mass)
{ 
	this->m_mass = mass;
}

void DirectX::Rigidbody::RegisterCollider(std::weak_ptr<Collider> collider)
{
	_Colliders.push_back(collider);
}

void Rigidbody::AddForce(Vector3 force) 
{ 
	this->m_velocity = this->m_mass * force; 
}

Vector3 DirectX::Rigidbody::GetVelocity()
{
	return this->m_velocity;
}

void DirectX::Rigidbody::SetVelocity(Vector3 velocity)
{
	this->m_velocity = velocity;
}
