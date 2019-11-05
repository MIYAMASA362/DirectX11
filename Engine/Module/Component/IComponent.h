#pragma once
namespace DirectX
{
	class Transform;
	class GameObject;

	class IComponent:public Object
	{
	private:
		const EntityID m_ownerId;

	public:
		IComponent(EntityID OwnerID);
		virtual ~IComponent();

		const EntityID GetOwnerID() const;
		virtual const ComponentTypeID GetComponentTypeID () const =0;

		virtual void DebugImGui() = 0;
		virtual void SendComponentMessage(std::string message)= 0;

		std::weak_ptr<Transform> transform();
		std::weak_ptr<GameObject> gameObject();

	protected:
		virtual void OnDestroy() override = 0;

	};

	inline const InstanceID DirectX::IComponent::GetOwnerID() const
	{
		return this->m_ownerId;
	}
}