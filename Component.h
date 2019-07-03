#pragma once

namespace DirectX
{
	class GameObject;
	class Transform;
	class Behaviour;

	/**
	@virtual
	+ Destroy()
	+ OnDestroy()
	+ OnComponent()
	+ SendBehaviourMessage(Message message)
	*/
	class Component:private Object
	{
		friend GameObject;
	public:
		enum Message
		{
			Initialize,
			Update,
			Render,
			Finalize
		};
	private:
		const int priority;
	protected:
		bool IsEnable = true;
	public:
		std::weak_ptr<GameObject> gameObject;
		std::weak_ptr<Transform> transform;
	public:
		Component(int priority) :Object(typeid(this).name()),priority(priority){};
		Component() :Component(0) {};
		virtual ~Component() {};
	public:
		void SetEnable(bool isEnable);
		bool GetEnable() { return IsEnable; };
		virtual void SendBehaviourMessage(Message message) {};
		virtual void OnComponent() {};
	};
}