#include<typeinfo>
#include"Common.h"

//DirectX
#include"Module\DirectX\DirectX.h"

//ECS
#include"../ECSEngine.h"

#include"Module\Transform\Transform.h"
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
	Entity(),
	_Scene(scene),
	_Name(name),
	_Tag(tagName),
	_IsDestroy(false),
	_IsActive(true)
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
		_transform = this->GetComponent<Transform>();
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
}

void GameObject::Register(std::shared_ptr<Object> instance)
{
	Entity::Register(instance);
	_gameObject = std::dynamic_pointer_cast<GameObject>(instance);
}
