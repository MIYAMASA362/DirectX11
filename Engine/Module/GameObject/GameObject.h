#pragma once

#include<functional>
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

	/*
		scene メンバ変数はSceneのCreanUp時に必要になる
	*/

	//ゲームオブジェクト :Entity
	class GameObject final:public Object
	{
	//--- Attribute -------------------------------------------------
		friend class SceneManager;
		friend class Scene;
	private:
		const std::string name;				//GameObject名
		std::weak_ptr<GameObject> self;		//Sceneが持っている自身へのポインタ
		Scene* const scene;					//所属Scene
		const Tag tag;
		bool IsDestroy = false;
		bool IsActive  = true;
	public:
		static const std::string TypeName;
	//--- Component -------------------------------------------------
	private:
		std::list<std::shared_ptr<Component>> Components;
	public:
		std::shared_ptr<Transform> transform;
		std::list<std::shared_ptr<Collider>> colliders;
		std::shared_ptr<Camera> camera;
		std::shared_ptr<MeshRender> meshRenderer;
		std::shared_ptr<Canvas> canvas;
		std::shared_ptr<Rigidbody>rigidbody;

	//--- Constructor/Destructor ------------------------------------
	public:
		GameObject(std::string name,Scene* scene, TagName tagName);
		GameObject(std::string name,Scene* scene) : GameObject(name, scene, TagName::Default) {};
		virtual ~GameObject();
	//--- Method ----------------------------------------------------
	public:
		//Componentにメッセージを走らせる
		void RunComponent(Component::Message message);
		void AddComponents(std::shared_ptr<Component> add);
	public:
		//タグ比較
		bool CompareTag(TagName tag);
		//削除されるか
		bool GetIsDestroy();
		//アクティブを設定
		void SetActive(bool IsActive);
		//アクティブか確認
		bool GetActive();
	//--- Component -------------------------------------------------
	public:
		//AddComponent
		template<typename Type> Type* AddComponent()
		{
			//既にあるのか確認
			{
				std::weak_ptr<Type> have = this->GetComponent<Type>();
				if (!have.expired())
				{
					OutputDebugString("重複しているComponentのAddComponentがあります。");
					return have.lock().get();
				}
			}

			Type* add = new Type();
			auto component = std::shared_ptr<Component>(add);

			//Component割り振り
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

			//Componentsへ追加
			AddComponents(component);

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
		//削除処理
		void Destroy();
	};
}