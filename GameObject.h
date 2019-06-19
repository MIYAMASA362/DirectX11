#pragma once

namespace DirectX
{
	class GameObject;
	class Scene;

	//ゲームオブジェクト :Entity
	class GameObject final
	{
	private:
		const std::string name;
		const Scene* scene;		//所属しているScene

		std::list<std::shared_ptr<Component>> Components;
	public:
		Transform transform;
	public:
		GameObject(std::string name,Scene* scene):name(name),scene(scene) {};
		virtual ~GameObject() { Components.clear(); };

	//--- Component --------------------------------
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