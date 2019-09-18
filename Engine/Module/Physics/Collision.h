#pragma once

namespace DirectX
{
	class Rigidbody;
	class GameObject;
	class Transform;
	class Collider;

	//=== Bounds ========================================================================
	class Bounds {
	private:
		Vector3 m_center;
		Vector3 m_size;
	public:
		Bounds(Vector3 center,Vector3 size);
		Vector3 GetCenter();
		Vector3 GetExtents();
		Vector3 GetMax();
		Vector3 GetMin();
		Vector3 GetSize();
		void SetCenter(Vector3 center);
		void SetSize(Vector3 size);
	};

	//-----------------------------------------------------------------------------------
	inline Vector3 Bounds::GetCenter()		{ return this->m_center; }
	inline Vector3 Bounds::GetExtents()	{ return this->m_size * 0.5f; }
	inline Vector3 Bounds::GetMax()		{ return this->m_center + this->GetExtents(); }
	inline Vector3 Bounds::GetMin()		{ return this->m_center - this->GetExtents(); }
	inline Vector3 Bounds::GetSize()		{ return this->m_size; }
	inline void Bounds::SetCenter(Vector3 center)	{ this->m_center = center; }
	inline void Bounds::SetSize(Vector3 size)		{ this->m_size = size; }

	//=== Collision =====================================================================
	class Collision
	{
	public:
		Collider* collider;
		Rigidbody* rigidbody;
		GameObject* gameObject;
		Transform* transform;
	};

	//=== Collider ======================================================================
	class Collider:public Component<Collider>
	{
		friend class SceneManager;
	public:
		//Collider‚ÌŽí—Þ
		enum ShapeType 
		{
			Box,
			Sphere
		};
	public:
		static void Hitjudgment(GameObject* gameObject,GameObject* otherObject);
	protected:
		static bool BoxVsBox(Collider* collider,Collider* other);
		static bool BoxVsShpere(Collider* collider,Collider* other);
		static bool SphereVsSphere(Collider* collider, Collider* other);
	public:
		Bounds bound;
		bool IsHit = false;			//Õ“Ë‚µ‚Ä‚¢‚é
		bool IsTrigger = false;		//•¨—‹““®‚·‚é‚©
	public:
		Collider();
		virtual ~Collider() = default;
		virtual ShapeType GetShapeType() = 0;	//Type
	public:
		virtual void Render()=0;
		void Center(Vector3 center);
	protected:
		virtual bool Judgment(Collider* other) = 0;
	};
	//-----------------------------------------------------------------------------------
	inline void Collider::Center(Vector3 center) { this->bound.SetCenter(center); }

	//=== SphereCollider ================================================================
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

	//=== BoxCollider ===================================================================
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