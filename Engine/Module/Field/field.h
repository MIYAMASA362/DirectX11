#pragma once

using namespace DirectX;
struct ID3D11Buffer;

class Texture;

//*********************************************************************************************************************
//
//	SkySphere
//
//*********************************************************************************************************************
class SkySphere final :public Mesh
{
private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	unsigned int m_IndexNum = 0;
public:
	Texture* m_Texture = nullptr;
public:
	SkySphere();
	~SkySphere();
	void Render(XMMATRIX& worldMatrix);
};

//*********************************************************************************************************************
//
//	MeshField
//
//*********************************************************************************************************************
class MeshField final :public Mesh
{
	friend class FieldCollider;
private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	unsigned int m_IndexNum = 0;
	unsigned int m_WidthNum = 0;
	unsigned int m_DepthNum = 0;
	float m_Width = 0.0f;
	float m_Depth = 0.0f;
public:
	Texture* m_Texture = nullptr;
public:
	MeshField();
	~MeshField();
	void Render(XMMATRIX& worldMatrix);
};

class MeshWall final :public Mesh
{
private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	unsigned int m_IndexNum = 0;
	float m_Scale = 0.0f;
public:
	Texture* m_Texture = nullptr;
public:
	MeshWall();
	~MeshWall();
	void Render(XMMATRIX& worldMatrix) ;
};

class MeshCircle final:public Mesh
{
private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	unsigned int m_IndexNum = 0;
public:
	Texture* m_Texture = nullptr;
public:
	MeshCircle();
	~MeshCircle();
	void Render(XMMATRIX& worldMatrix);
};