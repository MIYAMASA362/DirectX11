#include"Module\Module.h"

DirectX::Rigidbody::Rigidbody()
:
	Component("Rigidbody"),
	m_useGravity(true),
	m_velocity(Vector3::zero()),
	m_mass(1.0f)
{

}

DirectX::Rigidbody::~Rigidbody()
{

}

void DirectX::Rigidbody::IsUseGravity(bool enable)
{
	this->m_useGravity = enable;
}

void DirectX::Rigidbody::SendBehaviourMessage(Component::Message message)
{
	switch (message)
	{
	case DirectX::Component::Initialize:
		break;
	case DirectX::Component::Update:
		break;
	case DirectX::Component::FixedUpdate:
		FixedUpdate();
		break;
	case DirectX::Component::Render:
		break;
	case DirectX::Component::Finalize:
		break;
	default:
		break;
	}
}

void DirectX::Rigidbody::SetMass(float mass)
{
	this->m_mass = mass;
}

void DirectX::Rigidbody::FixedUpdate()
{
	Transform* m_transform = this->transform.lock().get();

	//�d��
	if(m_useGravity)
		m_velocity += m_mass * Physics::Get_Gravity();

	//y:0��n�ʂƂ�������
	//if (m_transform->position().y - (m_transform->scale().y*0.5f) + m_velocity.y < 0.0f)
		//m_velocity.y -= m_transform->position().y- (m_transform->scale().y* 0.5f) + m_velocity.y;
}

void DirectX::Rigidbody::AddForce(Vector3 force)
{
	this->m_velocity = this->m_mass * force;
}

const std::type_info & DirectX::Rigidbody::GetType()
{
	return typeid(*this);
}

void DirectX::Rigidbody::ApplyRigidbody()
{
	Transform* m_transform = this->transform.lock().get();
	m_transform->position(m_transform->position() + this->m_velocity);
}