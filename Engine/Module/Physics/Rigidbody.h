#pragma once

namespace DirectX
{
	//=== Rigidbody =====================================================================
	class Rigidbody :public Component<Rigidbody>
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
		void SetMass(float mass);
		void FixedUpdate();
		void AddForce(Vector3 force);
		Vector3 GetVelocity() { return m_velocity; };
		void SetVelocity(Vector3 velocity) { m_velocity = velocity; };
		void ApplyRigidbody();
	};

	//-----------------------------------------------------------------------------------
	inline void Rigidbody::IsUseGravity(bool enable) { this->m_useGravity = enable; }
	inline void Rigidbody::SetMass(float mass) { this->m_mass = mass; }
	inline void Rigidbody::AddForce(Vector3 force) { this->m_velocity = this->m_mass * force; }

}
