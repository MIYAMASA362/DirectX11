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
		//Component�Ƀ��b�Z�[�W�𑗐M���ꂽ���̓���
		std::function<void(std::string)> SendComponentMessage = {};
		//ImGui�̐ݒ�
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