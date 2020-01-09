#include<typeinfo>
#include"Common.h"

//DirectX
#include"Module\DirectX\DirectX.h"

//ECS
#include"../ECSEngine.h"

//Component Module
#include"Module\Transform\Transform.h"

//GameObject
#include"Module\Tag\Tag.h"
#include"GameObject.h"

#include"Module\Scene\SceneManager.h"

using namespace DirectX;

//*********************************************************************************************************************
//
//	GameObject
//
//*********************************************************************************************************************

//GameObject
//	コンストラクタ
//
GameObject::GameObject(std::string name, Scene* scene, TagName tagName)
	:
	name(name),
	tag(tagName),
	scene(scene),
	IsDestroy(false),
	IsActive(true)
{
	g_EntityIndex.emplace(GetEntityID(),std::dynamic_pointer_cast<GameObject>(Object::_self.lock()));
}

//~GameObject
//	デストラクタ
//
GameObject::~GameObject() 
{
	scene->RemoveSceneObject(this->GetInstanceID());
}

//Transform
//	
//
std::weak_ptr<Transform> GameObject::transform()
{
	if (_transform.expired())
		_transform = Transform::GetComponent(this->GetEntityID());
	return _transform;
}

std::weak_ptr<GameObject> GameObject::gameObject()
{
	return _gameObject;
}

void GameObject::OnDebugGUI()
{
	if(ImGui::TreeNode((name + " ID:" + std::to_string(this->GetEntityID())).c_str()))
	{
		ComponentManager::ImGui_ComponentView(this->GetEntityID());
		ImGui::TreePop();
	}
}

std::string GameObject::GetName()
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

bool GameObject::GetActiveSelf()
{
	return this->IsActive;
}

Scene * GameObject::GetScene()
{
	return scene;
}
