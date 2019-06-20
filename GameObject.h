#pragma once

namespace DirectX
{
	class GameObject;
	class Scene;

	//�Q�[���I�u�W�F�N�g :Entity
	class GameObject final
	{
	//--- Attribute -------------------------------------------------
	private:
		std::list<std::shared_ptr<Component>> Components;
	private:
		const std::string name;
		Scene* const scene;		//�������Ă���Scene
		const Tag tag;
	public:
		Transform transform;
	//--- Constructor/Destructor ------------------------------------
	public:
		GameObject(std::string name,Scene* scene,TagManager::TagName tagName):
			name(name),
			scene(scene),
			tag(tagName)
		{};
		GameObject(std::string name, Scene* scene) :
			GameObject(name, scene, TagManager::Default) {};

		virtual ~GameObject() { Components.clear(); };
	//--- Method ----------------------------------------------------
	public:
		bool CompareTag(TagManager::TagName tag) 
		{
			return this->tag.name == tag;
		};

	//--- Component -------------------------------------------------
	public:
		template<typename Type> Type* AddComponent()
		{
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

		void Destroy()
		{
			scene->RemoveObject(this);
			return;
		}

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