#pragma once

class Texture;
namespace DirectX
{

	class Rigidbody;
	class GameObject;
	class Transform;
	class Collider;

	class MeshField;
	class Mesh;
	class MeshCollider;

	class Bounds
	{
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
		float GetRadius();
		void SetCenter(Vector3 center);
		void SetSize(Vector3 size);
	public:
		void DebugImGui();
	};

	class Collider:public Component<Collider>
	{
	public:
		enum ShapeType
		{
			Box,
			Sphere,
			Field,
			Mesh
		};
	public:
		static void IsHitReset();
		static void Hitjudgment();
		static void Hitjudgment(std::list<std::weak_ptr<Collider>>& collider);
		static void Hitjudgment(std::list<std::weak_ptr<Collider>>& collider,std::list<std::weak_ptr<Collider>>& other);

	public:
		std::weak_ptr<Rigidbody> _rigidbody;

		Bounds bound;
		bool IsHit = false;				//Õ“Ë‚µ‚Ä‚¢‚é
		bool IsTrigger = false;			//•¨—‹““®‚·‚é‚©
		DirectX::Mesh* _mesh;

		Collider(EntityID OwnerID);
		virtual ~Collider();

		virtual ShapeType GetShapeType() = 0;	//Type
		
		void Start();
		virtual void Render() = 0;

		void Center(Vector3 center);

	protected:
		static bool BoxVsBox(Collider* collider,Collider* other);
		static bool OBBVsOBB(Collider* collider, Collider* other);
		static bool OBBVsShpere(Collider* collider, Collider* other);
		static bool BoxVsShpere(Collider* collider,Collider* other);
		static bool SphereVsSphere(Collider* collider, Collider* other);
		static bool SphereVsMesh(Collider* collider,Collider* mesh);

		virtual bool Judgment(Collider* other) = 0;
	};

	inline void Collider::Center(Vector3 center) { this->bound.SetCenter(center); }

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
		SphereCollider(EntityID OwnerID);
		virtual ~SphereCollider();

		ShapeType GetShapeType() override { return ShapeType::Sphere; };
		void Render() override;
		
		void SetRadius(float radius);

	protected:
		bool Judgment(Collider* other) override;

	};

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
		BoxCollider(EntityID OwnerID);
		virtual ~BoxCollider() = default;
		virtual ShapeType GetShapeType() override { return ShapeType::Box; };
		void SetSize(Vector3 size);
		void Render() override;
	protected:
		bool Judgment(Collider* other) override;
	};

	class FieldCollider final:public Collider
	{
	private:
		MeshField* _field;
	public:
		FieldCollider(EntityID OwnerID);
		ShapeType GetShapeType() override { return ShapeType::Field; };
		void SetMesh(MeshField* field);
		bool IsOnGround(Vector3 Position);
		float GetHeight(Vector3 Position);
		Vector3 GetNormal(Vector3 Position);
		void Render() override {};
		bool Judgment(Collider* other) override { return false; };
	};

	class MeshCollider final : public Collider
	{
		friend class Collider;
	public:
		MeshCollider(EntityID OwnerID);
		ShapeType GetShapeType() override { return ShapeType::Mesh; };
		void SetMesh(DirectX::Mesh* mesh);
		void Render() override {};
		bool Judgment(Collider* other) override { return false; };
	};
}