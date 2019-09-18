#pragma once
namespace DirectX
{
	using ComponentID = unsigned int;
	using EntityID = unsigned int;
	class Transform;
	class GameObject;

	//Component Interface
	class IComponent:public Object
	{
	protected:
		EntityID m_OwnerId;
		bool m_IsEnable;
	public:
		virtual ~IComponent() = default;
	public:
		void SetEnable(bool enable);
		bool GetEnable();
		EntityID GetOwnerID();
	public:
		virtual ComponentID GetComponentID() = 0;
		virtual void Run() = 0;
		virtual void DebugImGui() = 0;
		std::weak_ptr<Transform> transform();
		std::weak_ptr<GameObject> gameObject();
	public:
		virtual void OnDestroy() override;
	};

	//-------------------------------------------------------------------------
}