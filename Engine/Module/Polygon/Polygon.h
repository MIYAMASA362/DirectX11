#pragma once

class Texture;
struct ID3D11Buffer;

namespace DirectX
{
	class CPolygon :public Component<CPolygon>
	{
	private:
		ID3D11Buffer* m_VertexBuffer = nullptr;
		Texture** m_Texture = nullptr;
	public:
		void SendComponentMessage(std::string message) override;

		void Start();
		void Update();
		void Render();
		void Release() override;
	};
}