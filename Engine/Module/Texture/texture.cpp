#define _CRT_SECURE_NO_WARNINGS
#include"Common.h"

#include"Module\DirectX\DirectX.h"

#include"Module\AssetData\AssetData.h"
#include"texture.h"
#include"TextureManager.h"

void Texture::SetResource()
{
	D3DApp::GetDeviceContext()->PSSetShaderResources(0,1,&srv);
}
