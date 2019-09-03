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
		const std::string name;					//Component��
	protected:
		bool IsEnable = true;					//Component�̗L���E����
	public:
		std::weak_ptr<GameObject> gameObject;	//�A�^�b�`����Ă���GameObject�ւ̎Q��
		std::weak_ptr<Transform> transform;		//�A�^�b�`����Ă���GameObject��Transform�ւ̎Q��
	//--- Constrcutor/Destrcutor ------------------------------------
	public:
		Component(std::string name):name(name){};
		virtual ~Component() {};
	//--- Method ----------------------------------------------------
	public:
		void SetEnable(bool isEnable);
		bool GetEnable() { return IsEnable; };
	public:
		//Component�𑖂点��
		void Run(std::weak_ptr<GameObject> gameObject,std::weak_ptr<Transform> transform,Component::Message message);
		//Message�𑗂�
		virtual void SendBehaviourMessage(Message message) {};
		//Component���A�^�b�`���ꂽ��Ă΂��
		virtual void OnComponent() {};
		//Component���擾
		std::string GetComponentName() { return name; };
		virtual void OnDestroy() override {};
		virtual const std::type_info& GetType() = 0;
		virtual void DebugImGui();
	//�����蔻��nMessage
	public:
		virtual void OnTriggerEnter(std::weak_ptr<Collider> other) {};
		virtual void OnTriggerExit(std::weak_ptr<Collider> other) {};
		virtual void OnTriggerStay(std::weak_ptr<Collider> other) {};
		virtual void OnCollisionEnter(std::weak_ptr<Collision> collision) {};
		virtual void OnCollisionExit(std::weak_ptr<Collision> collision) {};
		virtual void OnCollisionStay(std::weak_ptr<Collision> collision) {};
	};
}