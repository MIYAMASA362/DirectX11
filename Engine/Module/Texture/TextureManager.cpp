#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<assert.h>
#include<string>
#include<memory>
#include<d3d11.h>
#include<DirectXMath.h>

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"Module\AssetData\AssetData.h"

#include"texture.h"
#include"TextureManager.h"

using namespace DirectX;

//--- TextureManager ----------------------------------------------------------

//Asset\Textureへのアクセス
const std::string TextureManager::AssetDataBase = AssetManager::AssetPath +"Texture/";
//Textureデータ
std::map<std::string, std::shared_ptr<Texture>> TextureManager::TextureIndex;

//Texture読み込み
Texture * DirectX::TextureManager::LoadTexture(const char* path)
{
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	//読み込み
	{
		unsigned char	header[18];
		unsigned char	*image;
		unsigned int	width, height;
		unsigned char	depth;
		unsigned int	bpp;
		unsigned int	size;

		// ヘッダ読み込み
		FILE* file;
		file = fopen(path, "rb");
		assert(file);

		fread(header, sizeof(header), 1, file);


		// 画像サイズ取得
		width = header[13] * 256 + header[12];
		height = header[15] * 256 + header[14];
		depth = header[16];

		if (depth == 32)
			bpp = 4;
		else if (depth == 24)
			bpp = 3;
		else
			bpp = 0;

		if (bpp != 4) {
			assert(false);
		}

		size = width * height * bpp;

		// メモリ確保
		image = (unsigned char*)new unsigned char[size];

		// 画像読み込み
		fread(image, size, 1, file);

		// R<->B
		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
				unsigned char c;
				c = image[(y * width + x) * bpp + 0];
				image[(y * width + x) * bpp + 0] = image[(y * width + x) * bpp + 2];
				image[(y * width + x) * bpp + 2] = c;
			}
		}

		fclose(file);


		D3D11_TEXTURE2D_DESC desc;
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = image;
		initData.SysMemPitch = width * 4;
		initData.SysMemSlicePitch = size;

		auto hr = D3DApp::GetDevice()->CreateTexture2D(&desc, &initData, &texture);
		if (FAILED(hr)) 
		{
			assert(false);
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
		SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;

		hr = D3DApp::GetDevice()->CreateShaderResourceView(texture, &SRVDesc, &srv);
		if (FAILED(hr))
		{
			assert(false);
		}

		delete image;
	}

	return new Texture(texture,srv);
}

//Assetを読み込み
void DirectX::TextureManager::LoadAsset(std::string name,std::string path)
{
	//TextureIndexへの追加
	TextureIndex.emplace(name, std::shared_ptr<Texture>(LoadTexture((AssetDataBase + path).c_str())));
}

//Assetを解放
void DirectX::TextureManager::Release()
{
	TextureIndex.clear();
}

//Asset内のTextureを取得
Texture* DirectX::TextureManager::GetTexture(std::string name)
{
	Texture* texture = TextureIndex[name].get();
	if (texture == nullptr)
		MessageBox(NULL,"Textureが存在しません。","TextureManager",MB_OK);
	return texture;
}