#pragma once

#include<typeinfo>

namespace DirectX
{
	class GameObject;
	class Scene;
	class Camera;
	class Collider;
	class MeshRender;
	class Canvas;
	class Rigidbody;

	//�Q�[���I�u�W�F�N�g :Entity
	class GameObject final:public Object
	{
		friend Scene;
	private:
		const std::string name;				//GameObject��
		std::weak_ptr<GameObject> self;		//Scene�������Ă��鎩�g�ւ̃|�C���^
		Scene* const scene;					//����Scene
		const Tag tag;
		bool IsDestroy = false;
		bool IsActive  = true;
	public:
		static const std::string TypeName;
	private:
		std::list<std::shared_ptr<Component>> Components;
	public:
		std::shared_ptr<Transform> transform;
		std::list<std::shared_ptr<Collider>> colliders;
		std::shared_ptr<Camera> camera;
		std::shared_ptr<MeshRender> meshRenderer;
		std::shared_ptr<Canvas> canvas;
		std::shared_ptr<Rigidbody>rigidbody;
	public:
		GameObject(std::string name,Scene* scene, TagName tagName);
		virtual ~GameObject();
	public:
		void RunComponent(Component::Message message);	//Component�Ƀ��b�Z�[�W�𑖂点��
		void SetActive(bool IsActive);					//�A�N�e�B�u��ݒ�
		bool CompareTag(TagName tag);					//�^�O��r
		bool GetIsDestroy();							//�폜����邩
		bool GetActive();								//�A�N�e�B�u���m�F
		void DebugGUI();
	public:
		//AddComponent
		template<typename Type> Type* AddComponent()
		{
			//���ɂ���̂��m�F
			{
				std::weak_ptr<Type> have = this->GetComponent<Type>();
				if (!have.expired())
				{
					OutputDebugString("�d�����Ă���Component��AddComponent������܂��B");
					return have.lock().get();
				}
			}

			Type* add = new Type();
			auto component = std::shared_ptr<Component>(add);

			//Component����U��
			{
				std::string TypeName = component->ObjectTypeName();
				auto &type = add->GetType();

				//Transform
				if(typeid(Transform) == type){
					this->transform = std::shared_ptr<Transform>(std::static_pointer_cast<Transform>(component));
				}
				//Colldier
				else if(typeid(Collider) == type){
					this->colliders.push_back(std::shared_ptr<Collider>(std::static_pointer_cast<Collider>(component)));
				}
				//Camera
				else if (typeid(Camera) == type) {
					this->camera = std::shared_ptr<Camera>(std::static_pointer_cast<Camera>(component));
				}
				//MeshRender
				else if(typeid(MeshRender)==type){
					this->meshRenderer = std::shared_ptr<MeshRender>(std::static_pointer_cast<MeshRender>(component));
				}
				//Canvas
				else if(typeid(Canvas) == type){
					this->canvas = std::shared_ptr<Canvas>(std::static_pointer_cast<Canvas>(component));
				}
				//Rigidbody
				else if(typeid(Rigidbody) == type){
					this->rigidbody = std::shared_ptr<Rigidbody>(std::static_pointer_cast<Rigidbody>(component));
				}
			}

			//Components�֒ǉ�
			{
				Components.push_back(component);
				component->gameObject = self;
				component->transform = self.lock()->transform;
				component->OnComponent();
			}
			return add;
		}
		//GetComponent
		template<typename Type> std::weak_ptr<Type> GetComponent()
		{
			for (std::shared_ptr<Component> component : Components)
			{
				if (typeid(Type) == component->GetType()) return std::static_pointer_cast<Type>(component);
			}
			return std::weak_ptr<Type>();
		}
		//�폜����
		void Destroy();
	};

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