#pragma once

namespace DirectX
{
	class GameObject;
	class Scene;

	//ゲームオブジェクト :Entity
	class GameObject final:public Object
	{
	//--- Attribute -------------------------------------------------
	private:
		std::list<std::shared_ptr<Component>> Components;
	private:
		const std::string name;
		const Tag tag;
		Scene* const scene;		//所属しているScene
		bool IsDestroy;
		bool IsActive;
	public:
		
		Transform transform;
	//--- Constructor/Destructor ------------------------------------
	public:
		//集約
		GameObject(std::string name, Scene* scene, TagManager::TagName tagName) :
			name(name),
			scene(scene),
			tag(tagName),
			IsDestroy(false),
			IsActive(true)
		{};

		GameObject(std::string name, Scene* scene) : GameObject(name, scene, TagManager::Default) {};

		virtual ~GameObject() 
		{ 
			Components.clear();
		};
	//--- Method ----------------------------------------------------
	public:
		bool CompareTag(TagManager::TagName tag) 
		{
			return this->tag.name == tag;
		};

		bool GetIsDestroy()
		{
			return IsDestroy;
		}

		void SetActive(bool IsActive)
		{
			this->IsActive = IsActive;
		}

	//--- Component -------------------------------------------------
	public:
		template<typename Type> Type* AddComponent()
		{
			Type* have = this->GetComponent<Type>();
			if (have)
			{
				OutputDebugString("重複しているComponentのAddComponentがあります。");
				return have;
			}
			std::shared_ptr<Component> component = std::shared_ptr<Component>(new Type());
			component->transform = &this->transform;
			component->gameObject = this;
			Components.push_back(component);
			return static_cast<Type*>(component._Get());
		};

		template<typename Type> Type* GetComponent()
		{
			for (std::shared_ptr<Component> component : Components)
				if (typeid(*component) ==  typeid(Type)) return static_cast<Type*>(component._Get());
			return NULL;
		};

		void Destroy() override;

		void Initialize()
		{
			for (std::shared_ptr<Component> component : Components)
			{
				if (!component->IsEnable) continue;
				component->gameObject = this;
				component->transform = &transform;
				component->Initialize();
			}
		}

		void Update()
		{
			if (!IsActive) return;

			for (std::shared_ptr<Component> component : Components)
			{
				if (!component->IsEnable) continue;
				component->gameObject = this;
				component->transform = &transform;
				component->Update();
			}
		}

		void Render()
		{
			if (!IsActive) return;

			for (std::shared_ptr<Component> component : Components)
			{
				if (!component->IsEnable) continue;
				component->gameObject = this;
				component->transform = &transform;
				component->Render();
			}
		}

		void Finalize()
		{
			for (std::shared_ptr<Component> component : Components)
			{
				component->gameObject = this;
				component->transform = &transform;
				component->Finalize();
			}
		}

	};
}