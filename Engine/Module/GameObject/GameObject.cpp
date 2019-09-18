#include<typeinfo>
#include<memory>
#include<map>
#include<vector>
#include<list>

#include<d3d11.h>
#include<DirectXMath.h>

//DirectX
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"Module\IMGUI\GUI_ImGui.h"

//ECS
#include"../ECSEngine.h"

//Component
#include"Module\Object\Object.h"
#include"Module\Component\Component.h"

//Component Module
#include"Module\Transform\Transform.h"

//GameObject
#include"Module\Tag\Tag.h"
#include"GameObject.h"

#include"Module\SystemManager\SystemManager.h"
#include"Module\Scene\SceneManager.h"

using namespace DirectX;

const std::string GameObject::TypeName = "GameObject";
std::list<EntityID> GameObject::m_EntityIndex;

GameObject::GameObject(std::string name, Scene* scene, TagName tagName)
:
	name(name),
	tag(tagName),
	scene(scene),
	IsDestroy(false),
	IsActive(true)
{
	this->AddIndex(this);
}

GameObject::~GameObject() 
{
	
}

void DirectX::GameObject::DebugGUI() 
{
	for(auto id : m_EntityIndex)
	{
		auto object = Entity<GameObject>::GetEntity(id);
		ImGui::Text(object.lock()->name.c_str());
	}
}

std::weak_ptr<Transform> DirectX::GameObject::transform()
{
	return ComponentManager::GetComponent<Transform>(this->m_EntityID);
}

