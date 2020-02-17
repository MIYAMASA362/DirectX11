#define INCLUDE_CEREAL
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
Rigidbody::Rigidbody()
:
	_UseGravity(true),
	_Velocity(Vector3::zero()),
	_Mass(1.0f)
{

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
	//重力
	if (_UseGravity)
		_Velocity += _Mass * Physics::Get_Gravity();
	else
		_Velocity = Vector3::zero();
}

//ApplyRigidbody
//
//
void Rigidbody::ApplyRigidbody()
{
	for (auto rigidbody : ComponentIndex) {
		auto transform = rigidbody.lock()->gameObject()->transform().lock();
		rigidbody.lock()->_OldPosition = transform->position();
		transform->position(transform->position() + rigidbody.lock()->_Velocity);
	}
}

//CollisionRigidbody
//
//
void Rigidbody::CollisionRigidbody()
{
	for(auto rigidbody : ComponentIndex)
	{
		Collider::Hitjudgment(rigidbody.lock()->_Colliders);
	}
}

void Rigidbody::OnDebugImGui()
{
	ImGui::Text("Rigidbody");
	ImGui::Checkbox("UseGravity:", &this->_UseGravity);
	ImGui::InputFloat("Mass:", &this->_Mass);
	ImGui::InputFloat3("Velocity:", &this->_Velocity.x);
	ImGui::InputFloat3("OldPosition", &this->_OldPosition.x);
	ImGui::TreePop();
}

void Rigidbody::SendComponentMessage(std::string message)
{
	if (message == "FixedUpdate")
		FixedUpdate();
}

