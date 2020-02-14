#define INCLUDE_CEREAL
#include"Common.h"
#include"Module\DirectX\DirectX.h"

#define NOT_INCLUDE_BEHAVIOUR
#include"Module\ECSEngine.h"

#include"Module\Transform\Transform.h"

#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"

//*********************************************************************************************************************
//
//	IComponent
//
//*********************************************************************************************************************

//IComponent
//	コンストラクタ
//
IComponent::IComponent()
	:
	Object()
{

}

//IComponent
//	コンストラクタ
//
IComponent::IComponent(EntityID OwnerID)
	:
	Object(),
	_Entity(EntityManager::GetInstance()->GetEntity(OwnerID).lock())
{

}

//~IComponent
//	デストラクタ
//
IComponent::~IComponent()
{
	if(_Entity) _Entity.reset();
}

ComponentTypeID IComponent::GetComponentTypeID()
{
	return typeid(*this).hash_code();
}

EntityID IComponent::GetOwnerID()
{
	return _Entity->GetEntityID();
}

//Transform
//	EntityのTransform
//
std::shared_ptr<Transform> IComponent::transform()
{
	return gameObject()->transform().lock();
}

//GameObject
//	EntityのGameObject
std::shared_ptr<GameObject> IComponent::gameObject()
{
	return std::static_pointer_cast<GameObject>(_Entity);
}

//OnDebugImGui
//
//
void IComponent::OnDebugImGui()
{
	ImGui::Text(("ID : " + std::to_string(this->GetInstanceID())).c_str());
	ImGui::Text(("OwnerID : " + std::to_string(_Entity->GetEntityID())).c_str());
}

//Destory
//	Componentの削除命令
//
void IComponent::Destroy()
{
	//削除登録
	ObjectManager::GetInstance()->DestroyObject(this);
	//Entity所有破棄
	_Entity.reset();
}

void IComponent::Release()
{

}
