#pragma once

namespace DirectX
{
	class Texture;

	//é≤ÉçÉbÉN
	struct Lock
	{
		bool x;
		bool y;
		bool z;
	};

	class Billboard:public Mesh
	{
	private:
		ID3D11Buffer* m_VertexBuffer = nullptr;
		ID3D11Buffer* m_IndexBuffer = nullptr;
		VERTEX_3D* m_VertexIndex = nullptr;
		unsigned int m_IndexNum = 0;
		float m_scale;
	public:
		Texture* m_Texture = nullptr;
		Lock lockAxis;
		bool IsPivotBottom = false;
	public:
		Billboard();
		~Billboard();
	public:
		void Render(XMMATRIX worldMatrix)override;
	};
}