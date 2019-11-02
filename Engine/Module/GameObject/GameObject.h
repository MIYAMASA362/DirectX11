#pragma once

#include<typeinfo>
#include"Module\Tag\Tag.h"

namespace DirectX
{
	class GameObject;
	class Scene;

	//--- GameObject ----------------------------------------------------------
	class GameObject final:public Entity<GameObject>
	{
		friend Scene;
	public:
		static const std::string TypeName;
		static void DebugGUI();
	private:
		const std::string name;
		const Tag tag;
		Scene* const scene;
		bool IsDestroy = false;
		bool IsActive  = true;
	public:
		GameObject(std::string name,Scene* scene, TagName tagName);
		virtual ~GameObject();
	public:
		void SetActive(bool IsActive);
		bool CompareTag(TagName tag);
		bool GetIsDestroy();
		bool GetActive();

		Transform* transform();

		void OnDebugGUI();
	};

	//----------------------------------------------------------------------------

	inline bool GameObject::CompareTag(TagName tag) {
		return this->tag.name == tag;
	};
	inline bool GameObject::GetIsDestroy() {
		return this->IsDestroy;
	}
	inline void GameObject::SetActive(bool IsActive) {
		this->IsActive = IsActive;
	};
	inline bool GameObject::GetActive() {
		return this->IsActive;
	}
}