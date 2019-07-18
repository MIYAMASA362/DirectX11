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
	colliders.clear();

	camera.reset();
	meshRenderer.reset();
	transform.reset();
};

void DirectX::GameObject::Destroy()
{
	scene->SetIsCeanUp(true);
	IsDestroy = true;
}

