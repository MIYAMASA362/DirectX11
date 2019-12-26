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

//Rigidbody
//	コンストラクタ
//
Rigidbody::Rigidbody(EntityID OwnerID)
:
	Component(OwnerID),
	_UseGravity(true),
	_Velocity(Vector3::zero()),
	_Mass(1.0f)
{

	this->SendComponentMessage = [this](std::string message)
	{
		if (message == "FixedUpdate") FixedUpdate();
	};
	
	this->OnDebugImGui = [this]() 
	{
		if (ImGui::TreeNode("Rigidbody")) 
		{
			ImGui::Checkbox("UseGravity:",&this->_UseGravity);
			ImGui::InputFloat("Mass:",&this->_Mass);
			ImGui::InputFloat3("Velocity:",&this->_Velocity.x);
			ImGui::InputFloat3("OldPosition",&this->_OldPosition.x);
			ImGui::TreePop();
		}
	};
}

//~Rigidbody
//	デストラクタ
//
Rigidbody::~Rigidbody()
{

}

//FixedUpdate
//	一定時間更新
//	重力を適応させる
//
void Rigidbody::FixedUpdate()
{
	Transform* m_transform = this->transform().get();

	//重力
	if (_UseGravity)
		_Velocity += _Mass * Physics::Get_Gravity();
	else
		_Velocity = Vector3::zero();

	//y:0を地面とした判定
	/*if (m_transform->position().y - (m_transform->scale().y*0.5f) + m_velocity.y < 0.0f)
		m_velocity.y -= m_transform->position().y- (m_transform->scale().y* 0.5f) + m_velocity.y;*/
}

//ApplyRigidbody
//
//
void Rigidbody::ApplyRigidbody()
{
	for (auto rigidbody : ComponentIndex) {
		auto transform = rigidbody.second->transform();
		rigidbody.second->_OldPosition = transform->position();
		transform->position(transform->position() + rigidbody.second->_Velocity);
	}
}

//CollisionRigidbody
//
//
void Rigidbody::CollisionRigidbody()
{
	for(auto rigidbody : ComponentIndex)
	{
		Collider::Hitjudgment(rigidbody.second->_Colliders);
	}
}

//OnDestroy
//
//
void Rigidbody::OnDestroy()
{
	//auto rigidbody = this->transform()->GetComponentInParent(Rigidbody::TypeID);

	//if(rigidbody.expired())
		

	/*Collider* collider = this->gameObject()->GetComponent<Collider>();
	if(collider)
		collider->IsAttachRigdbody = false;*/
}

