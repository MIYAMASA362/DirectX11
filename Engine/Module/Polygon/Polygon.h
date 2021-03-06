#pragma once

class Texture;
struct ID3D11Buffer;

namespace DirectX
{
	class CPolygon :public MonoBehaviour<CPolygon>
	{
	private:
		ID3D11Buffer* m_VertexBuffer = nullptr;
		Texture** m_Texture = nullptr;
	public:
		void Start() override;
		void Update() override;
		void Render();
		void OnDestroy() override;
	};
}