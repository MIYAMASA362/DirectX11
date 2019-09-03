#pragma once

namespace DirectX
{
	class Collider;
	class Collision;

	/**
	@virtual
	+ Initialize
	+ Update
	+ Finalize
	+ Destroy
	+ OnDestroy
	*/
	class Behaviour :public Component
	{
	public:
		Behaviour(std::string name):Component(name) {};
		virtual ~Behaviour(){};
	public:
		virtual const std::type_info& GetType() override { return typeid(*this); };
		virtual void SendBehaviourMessage(Component::Message message) override;
		//Defualt Method
		virtual void Initialize() {};
		virtual void Update() {};
		virtual void FixedUpdate() {};
		virtual void Finalize() {};
		virtual void OnDestroy() {};
		//“–‚½‚è”»’èŒnMessage
	public:
		virtual void OnTriggerEnter(std::weak_ptr<Collider> other) override {};
		virtual void OnTriggerExit(std::weak_ptr<Collider> other) override {};
		virtual void OnTriggerStay(std::weak_ptr<Collider> other) override {};
		virtual void OnCollisionEnter(std::weak_ptr<Collision> collision) override {};
		virtual void OnCollisionExit(std::weak_ptr<Collision> collision)  override {};
		virtual void OnCollisionStay(std::weak_ptr<Collision> collision) override {};
	};
}