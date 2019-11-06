#pragma once

#include<typeinfo>
#include"Module\Tag\Tag.h"

namespace DirectX
{
	class GameObject;
	class Scene;

	class GameObject final:public Entity<GameObject>
	{
		friend Scene;
	public:
		static const std::string TypeName;
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
}