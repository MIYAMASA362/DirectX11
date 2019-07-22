#include<memory>
#include<list>
#include<d3d11.h>
#include<DirectXMath.h>

//DirectX
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

//Component
#include"Module\Object\Object.h"
#include"Module\Component\Component.h"

//Component Module
#include"Module\Transform\Transform.h"

//GameObject
#include"Module\Tag\Tag.h"
#include"GameObject.h"

#include"Module\Scene\SceneManager.h"

using namespace DirectX;

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

