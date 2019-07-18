#pragma once

#include"Collision.h"
namespace DirectX
{
	class GameObject;
	class Scene;
	class Camera;
	class Collider;

	//�Q�[���I�u�W�F�N�g :Entity
	class GameObject final:public Object
	{
	//--- Attribute -------------------------------------------------
		friend class SceneManager;
		friend class Scene;
	private:
		std::weak_ptr<GameObject> self;
		Scene* const scene;	//����Scene
		const Tag tag;
		bool IsDestroy;
		bool IsActive;
	//--- Component --------------------------------
	private:
		std::list<std::shared_ptr<Component>> Components;
	public:
		std::shared_ptr<Transform> transform;
		std::list<std::shared_ptr<Component>> colliders;
		std::shared_ptr<Component> camera;
		std::shared_ptr<Component> meshRenderer;
	//--- Constructor/Destructor ------------------------------------
	public:
		GameObject(std::string name,Scene* scene, TagName tagName);
		GameObject(std::string name,Scene* scene) : GameObject(name, scene, TagName::Default) {};
		virtual ~GameObject();
	//--- Method ----------------------------------------------------
	private:
		//Component�Ƀ��b�Z�[�W�𑖂点��
		void RunComponent(Component::Message message)
		{
			for(auto component:Components){
				if (!component->GetEnable())return;
				component->gameObject = self;
				component->transform = transform;
				component->SendBehaviourMessage(message);
			}
		}
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
		bool GetActive() { 
			return IsActive;
		}
	//--- Component -------------------------------------------------
	public:
		//AddComponent
		template<typename Type> Type* AddComponent()
		{
			std::weak_ptr<Type> have = this->GetComponent<Type>();
			if (!have.expired())
			{
				OutputDebugString("�d�����Ă���Component��AddComponent������܂��B");
				return have.lock().get();
			}
			std::shared_ptr<Component> component = std::shared_ptr<Component>(new Type());
			Components.push_back(component);
			component->gameObject = self;
			component->transform = self.lock()->transform;
			component->OnComponent();

			//Camera
			if (Camera* camera = dynamic_cast<Camera*>(component.get()))
				this->camera = component;
			//Collider
			if (Collider* collider = dynamic_cast<Collider*>(component.get()))
				this->colliders.push_back(component);
			//MeshRenderer
			if (MeshRender* meshRenderer = dynamic_cast<MeshRender*>(component.get()))
				this->meshRenderer = component;

			return static_cast<Type*>(component.get());
		};

		//GetComponent
		template<typename Type> std::weak_ptr<Type> GetComponent()
		{
			for (std::shared_ptr<Component> component : Components)
				if (typeid(*component) == typeid(Type)) return std::static_pointer_cast<Type>(component);
			return std::weak_ptr<Type>();
		}


		//�폜����
		void Destroy();
	};
}