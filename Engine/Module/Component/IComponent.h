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
		const EntityID m_OwnerId;
	public:
		IComponent(EntityID OwnerID);
		virtual ~IComponent() = default;
	public:
		EntityID GetOwnerID();
	public:
		std::weak_ptr<Transform> transform();
		std::weak_ptr<GameObject> gameObject();
	public:
		virtual ComponentID GetComponentID() = 0;
		virtual void Run() = 0;
		virtual void OnDestroy() =0;
		virtual void DebugImGui() = 0;
	};

	//-------------------------------------------------------------------------
}