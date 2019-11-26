#pragma once
namespace DirectX
{
	class Transform;
	class GameObject;

	class IComponent:public Object
	{
		friend class ComponentManager;
	private:
		const EntityID _ownerId;
		std::weak_ptr<GameObject> _gameObject;
	protected:
		std::weak_ptr<IComponent> _self;
		//Componentにメッセージを送信された時の動作
		std::function<void(std::string)> SendComponentMessage = {};
		//ImGuiの設定
		std::function<void(void)> OnDebugImGui = {};
	public:
		IComponent(EntityID OwnerID);
		virtual ~IComponent();

		const EntityID GetOwnerID() const;
		virtual const ComponentTypeID GetComponentTypeID () const =0;

		std::shared_ptr<Transform> transform();
		std::shared_ptr<GameObject> gameObject();
	protected:
		virtual void OnDestroy() override = 0;

	};
}