#pragma once

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace DirectX
{
	//Texture
	class Texture
	{
		friend class TextureManager;
	private:
		ID3D11Texture2D* texture;
		ID3D11ShaderResourceView* srv;
	public:
		Texture()= default;
		~Texture();
	public:
		ID3D11Texture2D* GetTexture() { return this->texture; };
		ID3D11ShaderResourceView* GetShaderResourceView() { return this->srv; };
		void GetAsset(std::string name);
	};
}