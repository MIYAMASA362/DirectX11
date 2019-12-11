#pragma once

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

//Texture
class Texture
{
public:
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
public:
	Texture() = default;
	Texture(ID3D11Texture2D* texture, ID3D11ShaderResourceView* srv)
		:texture(texture),srv(srv){};
	~Texture() 
	{
		texture->Release();
		srv->Release();
	}
public:
	ID3D11Texture2D* GetTexture(){ return texture; };
	ID3D11ShaderResourceView* GetShaderResourceView() { return srv; };
	void SetResource();
};
