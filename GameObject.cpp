#include"main.h"
#include"Object.h"
#include"Component.h"
#include"Transform.h"
#include"Tag.h"
#include"Renderer.h"
#include"GameObject.h"
#include"Behaviour.h"
#include"camera.h"
#include"SceneManager.h"

std::shared_ptr<DirectX::Manager::GameObjectManager> DirectX::Manager::GameObjectManager::pInstance;

DirectX::Manager::GameObjectManager::GameObjectManager()
{
}

DirectX::Manager::GameObjectManager::~GameObjectManager()
{
	pIndex.clear();
}

void DirectX::Manager::GameObjectManager::Create()
{
	if (pInstance.get() == nullptr)
		pInstance = std::shared_ptr<GameObjectManager>(new GameObjectManager());
}

void DirectX::Manager::GameObjectManager::Destry()
{
	if (pInstance.get() != nullptr)
		pInstance.reset();
}




GameObject::GameObject(std::string name, Scene* scene, TagName tagName) :
	Object(name),
	scene(scene),
	tag(tagName),
	IsDestroy(false),
	IsActive(true)
{
	transform = std::shared_ptr<Transform>(new Transform());
	transform->transform = transform;
	Components.push_back(transform);
};

GameObject::~GameObject() {
	Components.clear();
	transform.reset();
};

void DirectX::GameObject::Destroy()
{
	scene->SetIsCeanUp(true);
	IsDestroy = true;
}

