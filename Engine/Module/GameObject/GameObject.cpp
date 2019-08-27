#include<typeinfo>
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

const std::string GameObject::TypeName = "GameObject";

GameObject::GameObject(std::string name, Scene* scene, TagName tagName)
:
	name(name),
	scene(scene),
	tag(tagName),
	IsDestroy(false),
	IsActive(true)
{
}

GameObject::~GameObject() 
{
	canvas.reset();
	camera.reset();
	meshRenderer.reset();
	transform.reset();
	colliders.clear();

	Components.clear();
}

void GameObject::RunComponent(Component::Message message)
{
	//Active‚Å‚È‚¢
	if (!this->IsActive) return;
	//Ž©g‚ÌComponent‚ð‘–‚ç‚·
	for (auto component : Components) {
		if (!component->GetEnable())continue;
		component->gameObject = self;
		component->transform = transform;
		component->SendBehaviourMessage(message);
	}
}

void DirectX::GameObject::AddComponents(std::shared_ptr<Component> add)
{
	Components.push_back(add);
	add->gameObject = self;
	add->transform = self.lock()->transform;
	add->OnComponent();
}

bool GameObject::CompareTag(TagName tag) {
	return this->tag.name == tag;
}

bool GameObject::GetIsDestroy() {
	return IsDestroy;
}

void GameObject::SetActive(bool IsActive) {
	this->IsActive = IsActive;
}

bool GameObject::GetActive() {
	return IsActive;
}

GameObject * DirectX::GameObject::Instantiate(GameObject * original)
{
	Transform* transform = original->transform.get();
	return GameObject::Instantiate(original,transform->localPosition(),transform->localScale(),transform->localRotation());
}

GameObject * DirectX::GameObject::Instantiate(GameObject* original, std::weak_ptr<Transform> parent)
{
	GameObject* instance = GameObject::Instantiate(original,parent.lock()->localPosition(),parent.lock()->localScale(),parent.lock()->localRotation());
	instance->transform->SetParent(parent);
	return instance;
}

GameObject * DirectX::GameObject::Instantiate(GameObject * original, Vector3 position, Vector3 scale, Quaternion rotation)
{
	GameObject* instance = SceneManager::GetActiveScene().lock()->AddSceneObject(original->name + "(clone)",TagName::Default);

	auto childindex = original->transform->GetChildren();
	for(auto itr = childindex.begin(); itr != childindex.end(); itr++)
		GameObject* child = GameObject::Instantiate(itr->lock()->gameObject.lock().get(),instance->transform);

	return instance;
}

void DirectX::GameObject::Destroy()
{
	scene->SetIsCeanUp(true);
	IsDestroy = true;
	for (auto child : transform->GetChildren())
		child.lock()->gameObject.lock()->IsDestroy = true;
}