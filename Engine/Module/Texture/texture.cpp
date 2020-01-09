#define _CRT_SECURE_NO_WARNINGS
#include"Common.h"
#include<fstream>

#define STB_IMAGE_IMPLEMENTATION
#include<stb\stb_image.h>

#include"Module\DirectX\DirectX.h"

#include"cereal\cereal.hpp"

#include"Module\Object\Object.h"

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
	D3DRenderer::GetInstance()->GetDeviceContext()->PSSetShaderResources(0,1,&_srv);
}

void Texture::LoadTexture(std::string filePath)
{
	HRESULT hr;
	unsigned char* pixels = nullptr; //画素

	int width;	//横
	int height;	//縦
	int bpp;	//色深度

	pixels = stbi_load(filePath.c_str(), &width, &height, &bpp, STBI_rgb_alpha);

	//リソースの設定
	D3D11_TEXTURE2D_DESC td;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	td.Width = width;
	td.Height = height;

	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = pixels;
	sd.SysMemPitch = width * 4;
	sd.SysMemSlicePitch = width * height * bpp;



	hr = D3DRenderer::GetInstance()->GetDevice()->CreateTexture2D(&td, &sd, &_texture);
	if (FAILED(hr))
		MessageBox(NULL, "テクスチャの読み込みに失敗しました。", "エラー", MB_OK);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;

	hr = D3DRenderer::GetInstance()->GetDevice()->CreateShaderResourceView(_texture, &srv, &_srv);
	if (FAILED(hr))
		MessageBox(NULL, "テクスチャのシェーダーリソースの作成に失敗しました。", "エラー", MB_OK);

	stbi_image_free(pixels);

#if 0
	HRESULT hr;

	//既にデータがあるか
	std::string path;
	size_t pos = filePath.find_last_of(".");
	if (pos != std::string::npos)
		path = filePath.substr(0, pos);
	path += ".data";

	unsigned char* pixels = nullptr; //画素

	int width;	//横
	int height;	//縦
	int bpp;	//色深度

	std::ifstream ifs(path);
	//リソースの設定
	D3D11_TEXTURE2D_DESC td;
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

	bool open = ifs.is_open();
	if(open)
	{
		ifs.read((char*)&width,sizeof(width));
		ifs.read((char*)&height, sizeof(height));
		ifs.read((char*)&bpp, sizeof(bpp));

		pixels = new unsigned char[width * height * bpp];
		ifs.read((char*)pixels, width * height * bpp);
		

		td.Width = width;
		td.Height = height;

		sd.pSysMem = pixels;
		sd.SysMemPitch = width * 4;
		sd.SysMemSlicePitch = width * height * bpp;
	}
	else
	{
		std::ofstream stream(path, std::ios::binary | std::ios::out);
		//読み込み
		pixels = stbi_load(filePath.c_str(), &width, &height, &bpp, STBI_rgb_alpha);
		
		td.Width = width;
		td.Height = height;
		

		sd.pSysMem = pixels;
		sd.SysMemPitch = width * 4;
		sd.SysMemSlicePitch = width * height * bpp;

		stream.write((char*)&width,sizeof(width));
		stream.write((char*)&height, sizeof(height));
		stream.write((char*)&bpp,sizeof(bpp));
		stream.write((char*)pixels, width * height * bpp);
		
		stream.close();
	}

	ifs.close();


	hr = D3DRenderer::GetInstance()->GetDevice()->CreateTexture2D(&td, &sd, &_texture);
	if (FAILED(hr))
		MessageBox(NULL, "テクスチャの読み込みに失敗しました。", "エラー", MB_OK);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;

	hr = D3DRenderer::GetInstance()->GetDevice()->CreateShaderResourceView(_texture, &srv, &_srv);
	if (FAILED(hr))
		MessageBox(NULL, "テクスチャのシェーダーリソースの作成に失敗しました。", "エラー", MB_OK);

	if(open)
	{
		delete[] pixels;
	}
	else
	{
		stbi_image_free(pixels);
	}

#endif


}