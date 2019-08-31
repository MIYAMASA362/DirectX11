#pragma once
#include<list>
#include<map>

namespace DirectX
{
	class GameObject;
	class Transform;
	class Behaviour;
	class Collision;
	class Collider;

	/**
	@override
	+ Destroy()
	+ OnDestroy()
	+ OnComponent()
	+ SendBehaviourMessage(Message message)
	+ GetComponentName()
	+ GetType()
	*/
	class Component:public Object
	{
		friend GameObject;
	//--- Attribute -------------------------------------------------
	public:
		//BehaviourMessage
		enum Message
		{
			Initialize,
			Update,
			FixedUpdate,
			Render,
			Finalize,
		};
	private:
		const std::string name;					//Component名
	protected:
		bool IsEnable = true;					//Componentの有効・無効
	public:
		std::weak_ptr<GameObject> gameObject;	//アタッチされているGameObjectへの参照
		std::weak_ptr<Transform> transform;		//アタッチされているGameObjectのTransformへの参照
	//--- Constrcutor/Destrcutor ------------------------------------
	public:
		Component(std::string name):name(name){};
		virtual ~Component() {};
	//--- Method ----------------------------------------------------
	public:
		void SetEnable(bool isEnable);
		bool GetEnable() { return IsEnable; };
	public:
		//Componentを走らせる
		void Run(std::weak_ptr<GameObject> gameObject,std::weak_ptr<Transform> transform,Component::Message message);
		//Messageを送る
		virtual void SendBehaviourMessage(Message message) {};
		//Componentがアタッチされたら呼ばれる
		virtual void OnComponent() {};
		//Component名取得
		std::string GetComponentName() { return name; };
		virtual void OnDestroy() override {};
		virtual const std::type_info& GetType() = 0;
		virtual void DebugImGui();
	//当たり判定系Message
	public:
		virtual void OnTriggerEnter(std::weak_ptr<Collider> other) {};
		virtual void OnTriggerExit(std::weak_ptr<Collider> other) {};
		virtual void OnTriggerStay(std::weak_ptr<Collider> other) {};
		virtual void OnCollisionEnter(std::weak_ptr<Collision> collision) {};
		virtual void OnCollisionExit(std::weak_ptr<Collision> collision) {};
		virtual void OnCollisionStay(std::weak_ptr<Collision> collision) {};
	};
}