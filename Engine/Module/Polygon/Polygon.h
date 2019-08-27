#pragma once

class Texture;
struct ID3D11Buffer;

namespace DirectX
{
	class CPolygon :public Behaviour
	{
	private:
		ID3D11Buffer* m_VertexBuffer = nullptr;
		Texture** m_Texture = nullptr;
	public:
		void Initialize() override;
		void Update() override;
		void Render();
		void Finalize() override;
	};
}