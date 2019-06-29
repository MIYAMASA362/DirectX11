#pragma once

namespace DirectX
{
	class GameObject;
	class Transform;
	class Behaviour;

	/**
	@virtual
	+ Initialize
	+ Update
	+ Render
	+ Finalize
	+ Destroy
	+ OnDestroy
	*/
	class Component:private Object
	{
		friend GameObject;
	protected:
		bool IsEnable = true;
	private:
		const int priority;
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
		virtual void Initialize() {};
		virtual void Update() {};
		virtual void Render() {};
		virtual void Finalize() {};
	};
}