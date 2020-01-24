#include<typeinfo>
#define INCLUDE_CEREAL
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

#include"Module\Hierarchy\Hierarchy.h"
#include"Module\Scene\Scene.h"
#include"Module\Scene\SceneManager.h"

using namespace DirectX;

//*********************************************************************************************************************
//
//	GameObject
//
//*********************************************************************************************************************

GameObject::GameObject()
	:
	Entity()
{

}

//GameObject
//	コンストラクタ
//
GameObject::GameObject(std::string name, Scene* scene, TagName tagName)
	:
	_Scene(scene),
	_Name(name),
	_Tag(tagName),
	_IsDestroy(false),
	_IsActive(true)
{
	
}

GameObject::GameObject(GameObject * gameObject,Scene* scene)
	:
	_Scene(scene),
	_Name(gameObject->_Name),
	_Tag(gameObject->_Tag),
	_IsDestroy(gameObject->_IsDestroy),
	_IsActive(gameObject->_IsActive)
{
	
}

//~GameObject
//	デストラクタ
//
GameObject::~GameObject() 
{
	
}

//Transform
//	
//
std::weak_ptr<Transform> GameObject::transform()
{
	if (_transform.expired())
		_transform = Transform::GetComponent(this->GetEntityID()).lock();
	return _transform;
}

//gameObject
//
//
std::weak_ptr<GameObject> GameObject::gameObject()
{
	return _gameObject;
}

//OnDebugGUI
//
//
void GameObject::OnDebugGUI()
{
	if(ImGui::TreeNode((_Name + " ID:" + std::to_string(this->GetEntityID())).c_str()))
	{
		ComponentManager::GetInstance()->ImGui_ComponentView(this->GetEntityID());
		ImGui::TreePop();
	}
}