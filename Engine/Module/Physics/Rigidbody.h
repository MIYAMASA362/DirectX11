#pragma once

namespace DirectX
{
	class Collision;

	class Rigidbody :public Component
	{
	private:
		bool m_useGravity;
		Vector3 m_velocity;
		float m_mass;
	public:
		Rigidbody();
		virtual ~Rigidbody();
	public:
		void IsUseGravity(bool enable);
		void SendBehaviourMessage(Component::Message message);
		void SetMass(float mass);
		void FixedUpdate();
		void AddForce(Vector3 force);
		const std::type_info& GetType()override;
		Vector3 GetVelocity() { return m_velocity; };
		void SetVelocity(Vector3 velocity) { m_velocity = velocity; };
		void ApplyRigidbody();
	};

}
