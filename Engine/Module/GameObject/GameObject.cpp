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

#include"Module\Scene\SceneManager.h"

using namespace DirectX;

EntityIndex GameObject::EntityType::m_EntityIndex;

GameObject::GameObject(std::string name, Scene* scene, TagName tagName)
	:
	name(name),
	tag(tagName),
	scene(scene),
	IsDestroy(false),
	IsActive(true)
{
	_gameObject = std::dynamic_pointer_cast<GameObject>(m_EntityIndex.at(this->GetEntityID()).lock());
}

GameObject::~GameObject() 
{
	scene->RemoveSceneObject(this->GetInstanceID());
}

std::weak_ptr<Transform> DirectX::GameObject::transform()
{
	if (_transform.expired())
		_transform = Transform::GetComponent(this->GetEntityID());
	return _transform;
}

std::weak_ptr<GameObject> DirectX::GameObject::gameObject()
{
	return _gameObject;
}

void DirectX::GameObject::OnDebugGUI()
{
	if(ImGui::TreeNode((name + " ID:" + std::to_string(this->GetEntityID())).c_str()))
	{
		ComponentManager::ImGui_ComponentView(this->GetEntityID());
		ImGui::TreePop();
	}
}

std::string DirectX::GameObject::GetName()
{
	return this->name;
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

bool DirectX::GameObject::GetActiveSelf()
{
	return this->IsActive;
}

Scene * DirectX::GameObject::GetScene()
{
	return scene;
}
