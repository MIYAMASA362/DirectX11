#define _CRT_SECURE_NO_WARNINGS
#include"Common.h"

#define STB_IMAGE_IMPLEMENTATION
#include<stb\stb_image.h>

#include"Module\DirectX\DirectX.h"

#include"Module\AssetData\AssetData.h"
#include"texture.h"
#include"TextureManager.h"

Texture::Texture()
{
	_texture = nullptr;
	_srv = nullptr;
}

Texture::Texture(ID3D11Texture2D * texture, ID3D11ShaderResourceView * srv)
	:
	_texture(texture),
	_srv(srv)
{

}

Texture::~Texture()
{
	if (_srv) _srv->Release();
	if (_texture) _texture->Release();
}

void Texture::SetResource()
{
	D3DApp::GetDeviceContext()->PSSetShaderResources(0,1,&_srv);
}

void Texture::LoadTexture(std::string filePath)
{
	HRESULT hr;
	unsigned char* pixels; //画素

	int width;	//横
	int height;	//縦
	int bpp;	//色深度

	//読み込み
	pixels = stbi_load(filePath.c_str(), &width, &height, &bpp, STBI_rgb_alpha);

	//リソースの設定
	{
		D3D11_TEXTURE2D_DESC td;
		td.Width = width;
		td.Height = height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = pixels;
		sd.SysMemPitch = width * 4;
		sd.SysMemSlicePitch = width * height * bpp;

		hr = D3DApp::GetDevice()->CreateTexture2D(&td, &sd, &_texture);
		if (FAILED(hr))
			MessageBox(NULL, "テクスチャの読み込みに失敗しました。", "エラー", MB_OK);

		D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
		srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srv.Texture2D.MipLevels = 1;

		hr = D3DApp::GetDevice()->CreateShaderResourceView(_texture, &srv, &_srv);
		if (FAILED(hr))
			MessageBox(NULL, "テクスチャのシェーダーリソースの作成に失敗しました。", "エラー", MB_OK);
	}

	//解放
	stbi_image_free(pixels);

}
