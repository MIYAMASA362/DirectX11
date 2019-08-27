#pragma once

using namespace DirectX;
struct ID3D11Buffer;
namespace DirectX
{
	class Texture;

	class CField :public Mesh
	{
	private:
		ID3D11Buffer* m_VertexBuffer = nullptr;
	public:
		Texture* m_Texture = nullptr;
		CField();
		virtual ~CField();
		void Render(XMMATRIX worldMatrix) override;
	};

	class WallField :public Mesh
	{
	private:
		ID3D11Buffer* m_FieldVertexBuffer = nullptr;
		ID3D11Buffer* m_WallVertexBuffer = nullptr;
	public:
		Texture* FieldTexture = nullptr;
		Texture* WallTexture = nullptr;
	public:
		WallField();
		~WallField();
		void Render(XMMATRIX worldMatrix)override;
	};


	class SkySphere final :public Mesh
	{
	private:
		ID3D11Buffer* m_VertexBuffer = nullptr;
		ID3D11Buffer* m_IndexBuffer = nullptr;
		VERTEX_3D* m_pVertexIndex = nullptr;
		unsigned int m_IndexNum = 0;
	public:
		Texture* m_Texture = nullptr;
	public:
		SkySphere();
		~SkySphere();
		void Render(XMMATRIX worldMatrix) override;
	};

	class MeshField final :public Mesh
	{
	private:
		ID3D11Buffer* m_VertexBuffer = nullptr;
		ID3D11Buffer* m_IndexBuffer = nullptr;
		VERTEX_3D* m_VertexIndex = nullptr;
		unsigned int m_IndexNum = 0;
	public:
		Texture* m_Texture = nullptr;
	public:
		MeshField();
		~MeshField();
		void Render(XMMATRIX worldMatrix)override;
	};

	class MeshWall final :public Mesh
	{
	private:
		ID3D11Buffer* m_VertexBuffer = nullptr;
		ID3D11Buffer* m_IndexBuffer = nullptr;
		VERTEX_3D* m_pVertexIndex = nullptr;
		unsigned int m_IndexNum = 0;
		float m_Scale = 0.0f;
	public:
		Texture* m_Texture = nullptr;
	public:
		MeshWall();
		~MeshWall();
		void Render(XMMATRIX worldMatrix) override;
	};

	class MeshCircle final:public Mesh
	{
	private:
		ID3D11Buffer* m_VertexBuffer = nullptr;
		ID3D11Buffer* m_IndexBuffer = nullptr;
		VERTEX_3D* m_pVertexIndex = nullptr;
		unsigned int m_IndexNum = 0;
	public:
		Texture* m_Texture = nullptr;
	public:
		MeshCircle();
		~MeshCircle();
		void Render(XMMATRIX worldMatrix)override;
	};
}