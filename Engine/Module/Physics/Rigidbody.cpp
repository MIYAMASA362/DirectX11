#include"Common.h"

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"Module\ECSEngine.h"

#include"Module\Transform\Transform.h"
#include"Physics.h"
#include"Rigidbody.h"

using namespace DirectX;

Rigidbody::ComponentIndex Rigidbody::ComponentType::Index;

Rigidbody::Rigidbody(EntityID OwnerID)
:
	Component(OwnerID),
	m_useGravity(true),
	m_velocity(Vector3::zero()),
	m_mass(1.0f)
{

}

Rigidbody::~Rigidbody()
{

}

void Rigidbody::FixedUpdate()
{
	Transform* m_transform = this->transform().lock().get();

	//d—Í
	if(m_useGravity)
		m_velocity += m_mass * Physics::Get_Gravity();

	//y:0‚ð’n–Ê‚Æ‚µ‚½”»’è
	//if (m_transform->position().y - (m_transform->scale().y*0.5f) + m_velocity.y < 0.0f)
		//m_velocity.y -= m_transform->position().y- (m_transform->scale().y* 0.5f) + m_velocity.y;
}

void Rigidbody::ApplyRigidbody()
{
	Transform* m_transform = this->transform().lock().get();
	m_transform->position(m_transform->position() + this->m_velocity);
}

void DirectX::Rigidbody::SendComponentMessage(std::string message)
{
	if (message == "FixedUpdate") 
	{
		FixedUpdate();
	}
}

void Rigidbody::IsUseGravity(bool enable) 
{
	this->m_useGravity = enable;
}

void Rigidbody::SetMass(float mass)
{ 
	this->m_mass = mass;
}

void Rigidbody::AddForce(Vector3 force) 
{ 
	this->m_velocity = this->m_mass * force; 
}
