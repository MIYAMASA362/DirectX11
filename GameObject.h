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

		std::list<Component*> Components;
	public:
		Transform transform;
	public:
		GameObject(std::string name,Scene* scene):name(name),scene(scene) {};
		virtual ~GameObject()
		{
			for (Component* component : Components)
				delete component;
			Components.clear();
		};

	//--- Component --------------------------------
	public:
		template<typename Type> void AddComponent()
		{
			Type* type = new Type();
			Components.push_back(type);
		};

		template<typename Type> Type* GetComponent()
		{
			for (Component* component : Components)
				if (typeid(*component) ==  typeid(Type)) return static_cast<Type*>(component);
			return NULL;
		};

		void Initialize()
		{
			for (Component* component : Components)
			{
				if (!component->IsEnable) continue;
				component->gameObject = this;
				component->transform = &transform;
				component->Initialize();
			}
		}

		void Update()
		{
			for (Component* component : Components)
			{
				if (!component->IsEnable) continue;
				component->gameObject = this;
				component->transform = &transform;
				component->Update();
			}
		}

		void Render()
		{
			for (Component* component : Components)
			{
				if (!component->IsEnable) continue;
				component->gameObject = this;
				component->transform = &transform;
				component->Render();
			}
		}

		void Finalize()
		{
			for (Component* component : Components)
			{
				component->gameObject = this;
				component->transform = &transform;
				component->Finalize();
			}
		}

	};
}