#pragma once

namespace DirectX
{
	class Rigidbody;
	class GameObject;
	class Transform;

	//Bounds
	class Bounds {
	private:
		Vector3 m_center;
		Vector3 m_size;
	public:
		Bounds(Vector3 center,Vector3 size);
		Vector3 GetCenter()	{ return this->m_center; };
		Vector3 GetExtents() { return this->m_size * 0.5f; };
		Vector3 GetMax() { return this->m_center + this->GetExtents(); };
		Vector3 GetMin() { return this->m_center - this->GetExtents(); };
		Vector3 GetSize(){ return this->m_size; };
		void SetCenter(Vector3 center) { this->m_center = center; };
		void SetSize(Vector3 size) { this->m_size = size; };
	};

	//Collision
	class Collision
	{
	public:
		Collider* collider;
		Rigidbody* rigidbody;
		GameObject* gameObject;
		Transform* transform;
	};

	//Collider
	class Collider:public Component
	{
		friend class SceneManager;
	public:
		//Collider‚ÌŽí—Þ
		enum ShapeType {
			Box,
			Sphere
		};
	public:
		Bounds bound;
		bool IsHit = false;			//Õ“Ë‚µ‚Ä‚¢‚é
		bool IsTrigger = false;		//•¨—‹““®‚·‚é‚©
	public:
		Collider();
		virtual ~Collider() = default;
		virtual ShapeType GetShapeType() = 0;	//Type
	public:
		virtual const std::type_info& GetType() override { return typeid(Collider); };
		virtual void Render()=0;
		void Center(Vector3 center) { this->bound.SetCenter(center); };
		void SendBehaviourMessage(Message message) override;
		static void Hitjudgment(GameObject* gameObject,GameObject* otherObject);
	protected:
		virtual bool Judgment(Collider* other) = 0;
		static bool BoxVsBox(Collider* collider,Collider* other);
		static bool BoxVsShpere(Collider* collider,Collider* other);
		static bool SphereVsSphere(Collider* collider, Collider* other);
	};

	//Sphere
	class SphereCollider final:public Collider
	{
	private:
		static ID3D11Buffer* m_VertexBuffer;
		static ID3D11Buffer* m_IndexBuffer;
		static VERTEX_3D* m_pVertex;
		static int m_IndexNum;
		static Texture* m_Texture;
	public:
		static void SetRenderBuffer();
		static void ReleaseRenderBuffer();
	public:
		SphereCollider();
		virtual ~SphereCollider() = default;
		virtual ShapeType GetShapeType() override { return ShapeType::Sphere; };
		void SetRadius(float radius);
		void Render() override;
	protected:
		bool Judgment(Collider* other) override;
	};

	//Box
	class BoxCollider final:public Collider
	{
	private:
		static ID3D11Buffer* m_IndexBuffer;
		static ID3D11Buffer* m_VertexBuffer;
		static VERTEX_3D* m_pVertex;
		static int m_IndexNum;
		static Texture* m_Texture;
	public:
		static void SetRenderBuffer();
		static void ReleaseRenderBuffer();
	public:
		BoxCollider() = default;
		virtual ~BoxCollider() = default;
		virtual ShapeType GetShapeType() override { return ShapeType::Box; };
		void SetSize(Vector3 size);
		void Render() override;
	protected:
		bool Judgment(Collider* other) override;
	};
}