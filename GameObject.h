#pragma once

namespace DirectX
{
	class GameObject;
	class Scene;
	class Camera;

	//ゲームオブジェクト :Entity
	class GameObject final:public Object
	{
	//--- Attribute -------------------------------------------------
		friend class SceneManager;
		friend class Scene;
	private:
		std::weak_ptr<GameObject> self;
		std::list<std::shared_ptr<Component>> Components;
		Scene* const scene;	//所属Scene
		const Tag tag;
		bool IsDestroy;
		bool IsActive;
	public:
		std::shared_ptr<Transform> transform;
	//--- Constructor/Destructor ------------------------------------
	public:
		GameObject(std::string name,Scene* scene, TagName tagName);
		GameObject(std::string name,Scene* scene) : GameObject(name, scene, TagName::Default) {};
		virtual ~GameObject();
	//--- Method ----------------------------------------------------
	public:
		bool CompareTag(TagName tag) 
		{
			return this->tag.name == tag;
		};
		bool GetIsDestroy()
		{
			return IsDestroy;
		};
		void SetActive(bool IsActive)
		{
			this->IsActive = IsActive;
		};
		bool GetActive() { return IsActive; }
	//--- Component -------------------------------------------------
	public:
		//AddComponent
		template<typename Type> Type* AddComponent()
		{
			std::weak_ptr<Type> have = this->GetComponent<Type>();
			if (!have.expired())
			{
				OutputDebugString("重複しているComponentのAddComponentがあります。");
				return have.lock().get();
			}
			std::shared_ptr<Component> component = std::shared_ptr<Component>(new Type());
			Components.push_back(component);
			component->gameObject = self;
			component->transform = self.lock()->transform;
			component->OnComponent();
			return static_cast<Type*>(component.get());
		};
		//GetComponent
		template<typename Type> std::weak_ptr<Type> GetComponent()
		{
			for (std::shared_ptr<Component> component : Components)
				if (typeid(*component) == typeid(Type)) return std::static_pointer_cast<Type>(component);
			return std::weak_ptr<Type>();
		}
		//削除処理
		void Destroy()
			;
	};
}