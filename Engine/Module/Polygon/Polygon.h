#pragma once

class CTexture;
struct ID3D11Buffer;

namespace DirectX
{
	class CPolygon :public Behaviour
	{
	private:
		ID3D11Buffer* m_VertexBuffer = NULL;
		CTexture* m_Texture = NULL;
	public:
		void Initialize() override;
		void Update() override;
		void Render();
		void Finalize() override;
	};
}