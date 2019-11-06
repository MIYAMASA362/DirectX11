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
		Rigidbody(EntityID OwnerID);
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
}
