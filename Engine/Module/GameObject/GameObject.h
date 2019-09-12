#pragma once

#include<typeinfo>

namespace DirectX
{
	class GameObject;
	class Scene;

	//--- GameObject ----------------------------------------------------------
	class GameObject final:public Entity<GameObject>
	{
		friend Scene;
	private:
		const std::string name;
		const Tag tag;
		Scene* const scene;
	private:
		std::weak_ptr<GameObject> self;
		bool IsDestroy = false;
		bool IsActive  = true;
	public:
		static const std::string TypeName;
	public:
		GameObject(std::string name,Scene* scene, TagName tagName);
		virtual ~GameObject();
	public:
		void SetActive(bool IsActive);
		bool CompareTag(TagName tag);
		bool GetIsDestroy();
		bool GetActive();
		void DebugGUI();
	public:
		std::weak_ptr<Transform> transform();
	public:
		template<typename Type> Type* AddComponent();
		template<typename Type> std::weak_ptr<Type> GetComponent();
	};

	//----------------------------------------------------------------------------
	template<typename Type> Type* GameObject::AddComponent()
	{
		return ComponentManager::AddComponent<Type>(this->m_EntityID);
	}
	template<typename Type> std::weak_ptr<Type> GameObject::GetComponent()
	{
		return ComponentManager::GetComponent<Type>(this->m_EntityID);
	}
	bool GameObject::CompareTag(TagName tag) {
		return this->tag.name == tag;
	};
	bool GameObject::GetIsDestroy() {
		return this->IsDestroy;
	}
	void GameObject::SetActive(bool IsActive) {
		this->IsActive = IsActive;
	};
	bool GameObject::GetActive() {
		return this->IsActive;
	}
}