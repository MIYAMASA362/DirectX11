#pragma once

namespace DirectX
{
	class Collider;

	class Rigidbody final :public Component<Rigidbody>
	{
	private:
		bool m_useGravity;
		Vector3 m_velocity;
		float m_mass;

		Vector3 m_oldPosition;
		std::list<std::weak_ptr<Collider>> _Colliders;
	public:
		static void ApplyRigidbody();
		static void CollisionRigidbody();

		Rigidbody(EntityID OwnerID);
		virtual ~Rigidbody();

		void IsUseGravity(bool enable);
		
		Vector3 GetVelocity();
		void SetVelocity(Vector3 velocity);
		
		void SetMass(float mass);
		void RegisterCollider(std::weak_ptr<Collider> collider);

		void AddForce(Vector3 force);
		Vector3 GetOldPosition();

		void FixedUpdate();
	protected:
		void OnDestroy() override;
	};
}
