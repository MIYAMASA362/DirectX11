#pragma once

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

namespace DirectX
{
	//BaseTexture
	class BaseTexture
	{
	public:
		virtual ID3D11Texture2D* GetTexture()=0;
		virtual ID3D11ShaderResourceView* GetShaderResourceView() =0;
	};

	//Texture
	class Texture :public BaseTexture
	{
	protected:
		ID3D11Texture2D* texture = nullptr;
		ID3D11ShaderResourceView* srv = nullptr;
	public:
		Texture(ID3D11Texture2D* texture, ID3D11ShaderResourceView* srv)
			:texture(texture),srv(srv){};
		~Texture() 
		{
			texture->Release();
			srv->Release();
		}
	public:
		ID3D11Texture2D* GetTexture() override { return texture; };
		ID3D11ShaderResourceView* GetShaderResourceView() override { return srv; };
		void SetResource();
	};
}