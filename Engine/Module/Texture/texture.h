#pragma once

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace DirectX
{
	//Texture
	class Texture
	{
	private:
		ID3D11Texture2D* texture;
		ID3D11ShaderResourceView* srv;
	public:
		Texture()= default;
		Texture(ID3D11Texture2D* texture, ID3D11ShaderResourceView* srv) :texture(texture), srv(srv) {};
		~Texture();
	public:
		ID3D11Texture2D* GetTexture() { return this->texture; };
		ID3D11ShaderResourceView* GetShaderResourceView() { return this->srv; };
	};
}