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


//~IComponent
//	デストラクタ
//
IComponent::~IComponent()
{
	
}

GameObject* IComponent::gameObject()
{
	return static_cast<GameObject*>(_Entity);
}

EntityID IComponent::GetOwnerID()
{
	return gameObject()->GetEntityID();
}

//OnDebugImGui
//
//
void IComponent::OnDebugImGui()
{
	ImGui::Text(("ID : " + std::to_string(this->GetInstanceID())).c_str());
}

//Release
//
//
void IComponent::Release()
{
	ComponentManager::GetInstance()->ReleaseComponent(this);
	//Entity._Componentsから削除
	_Entity->ReleaseComponent(this);
}

void IComponent::Register(std::shared_ptr<Object> instance)
{
	Object::Register(instance);
	ComponentManager::GetInstance()->RegisterComponent(std::dynamic_pointer_cast<IComponent>(instance));
}
