#pragma once

namespace DirectX
{
	class GameObject;
	class Transform;

	class Component
	{
		friend GameObject;
	private:
		bool IsEnable = true;
		const int priority;
	public:
		GameObject* gameObject;
		Transform* transform;
	public:
		Component():priority(0) {};
		Component(int priority):priority(priority) {};
		virtual ~Component() {};
	public:
		void SetEnable(bool isEnable);
	public:
		virtual void Initialize() {};
		virtual void Update() {};
		virtual void Render() {};
		virtual void Finalize() {};
	};
}