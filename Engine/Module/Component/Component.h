#pragma once

namespace DirectX
{
	/*
	@override
	+ Destroy()
	+ OnDestroy()
	+ OnComponent()
	+ SendBehaviourMessage(Message message)
	+ GetComponentName()
	+ GetType()
	
	class Component:public Object
	{
		friend GameObject;
	//--- Attribute -------------------------------------------------
	public:
		//BehaviourMessage
		enum Message{
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
	};*/

	using ComponentID = unsigned int;
	using EntityID = unsigned int;

	template<typename Type>
	class Component:public IComponent
	{
	protected:
		static ComponentID m_id;
		static std::map<EntityID, std::shared_ptr<Type>> ComponentIndex;
	public:
		static std::weak_ptr<Type> AddComponent(EntityID id);
		static std::weak_ptr<Type> GetComponent(EntityID id);
		static void DestroyComponent(EntityID id);
		static ComponentID GetID();
	protected:
		const std::string name;
	public:
		Component(std::string name);
		virtual ~Component();
	public:
		virtual void Run() override;
		virtual void OnComponent();
		virtual void OnDestroy() override;
		virtual void DebugImGui() override;
	public:
		ComponentID GetComponentID()override final;
	};

	//----------------------------------------------------------------------------
	template<typename Type>
	ComponentID Component<Type>::m_id = ComponentManager::CreateComponent<Type>();

	template<typename Type>
	inline std::weak_ptr<Type> Component<Type>::AddComponent(EntityID id)
	{
		auto add = ComponentManager::AddComponent<Type>(id);
		add->m_OwnerId = id;
		ComponentIndex.emplace(id,std::shared_ptr<Type>(add));
		return add;
	}

	template<typename Type>
	inline std::weak_ptr<Type> Component<Type>::GetComponent(EntityID id)
	{
		return ComponentIndex.at(id);
	}

	template<typename Type> 
	inline void Component<Type>::DestroyComponent(EntityID id)
	{
		ComponentIndex.erase(id);
	}

	template<typename Type>
	inline ComponentID Component<Type>::GetID()
	{
		return m_id;
	}
	template<typename Type>
	inline ComponentID Component<Type>::GetComponentID()
	{
		return this->m_id;
	}

	template<typename Type>
	inline Component<Type>::Component(std::string name)
	:
		name(name)
	{

	}

	template<typename Type>
	inline Component<Type>::~Component()
	{
		
	}

	template<typename Type>
	inline void Component<Type>::Run()
	{
	
	}

	template<typename Type>
	inline void Component<Type>::DebugImGui()
	{
		
	}

	template<typename Type>
	inline void Component<Type>::OnComponent()
	{
		
	}
	template<typename Type>
	inline void Component<Type>::OnDestroy()
	{
	}
}