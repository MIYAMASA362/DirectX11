#include<typeinfo>
#include"Common.h"
//DirectX
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"Module\IMGUI\GUI_ImGui.h"

//ECS
#include"../ECSEngine.h"

//Component Module
#include"Module\Transform\Transform.h"

//GameObject
#include"Module\Tag\Tag.h"
#include"GameObject.h"

#include"Module\SystemManager\SystemManager.h"
#include"Module\Scene\SceneManager.h"

using namespace DirectX;

const std::string GameObject::TypeName = "GameObject";
EntityIndex GameObject::EntityType::m_EntityIndex;

GameObject::GameObject(std::string name, Scene* scene, TagName tagName)
:
	name(name),
	tag(tagName),
	scene(scene),
	IsDestroy(false),
	IsActive(true)
{

}

GameObject::~GameObject() 
{

}

Transform* DirectX::GameObject::transform()
{
	return Transform::GetComponent(this->GetEntityID()).lock().get();
}

void DirectX::GameObject::OnDebugGUI()
{
	if(ImGui::TreeNode((name + " ID:" + std::to_string(this->GetEntityID())).c_str()))
	{
		auto components = ComponentManager::GetComponents(this->GetEntityID());
		if (components.expired()) return;
		for (auto component : *components.lock())
			component.second.lock()->DebugImGui();
		ImGui::TreePop();
	}
}

bool GameObject::CompareTag(TagName tag)
{
	return this->tag.name == tag;
}

bool GameObject::GetIsDestroy()
{
	return this->IsDestroy;
}

void GameObject::SetActive(bool IsActive)
{
	this->IsActive = IsActive;
}

bool GameObject::GetActive() 
{
	return this->IsActive;
}
