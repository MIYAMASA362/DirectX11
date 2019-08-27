#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<assert.h>
#include<map>
#include<string>
#include<memory>
#include<d3d11.h>
#include<DirectXMath.h>

//DirectX
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"Module\Material\Material.h"
#include"Module\Texture\texture.h"

//Component
#include"Module\Object\Object.h"
#include"Module\Component\Component.h"

//Component Module
#include"Module\Renderer\Renderer.h"

#include"model.h"

#include"ModelManager.h"

#include"Module\Texture\TextureManager.h"

DirectX::Model::Model()
{
	offsetMatrix = XMMatrixIdentity();
}

//•`‰æ
void DirectX::Model::Render(XMMATRIX worldMatrix)
{
	if(IsCulling)
		D3DApp::Renderer::SetRasterize(D3D11_FILL_SOLID,D3D11_CULL_NONE);
	
	worldMatrix = offsetMatrix * worldMatrix;

	D3DApp::Renderer::SetWorldMatrix(&worldMatrix);

	D3DApp::Renderer::SetVertexBuffer(VertexBuffer);
	D3DApp::Renderer::SetIndexBuffer(IndexBuffer);

	for(unsigned short i = 0; i<SubsetNum; i++)
	{
		D3DApp::Renderer::SetMaterial(SubsetArray[i].Material.Material);

		D3DApp::Renderer::SetTexture(SubsetArray[i].Material.Texture);
		
		D3DApp::Renderer::DrawIndexed(SubsetArray[i].IndexNum, SubsetArray[i].StartIndex,0);
	}
	if(IsCulling)
		D3DApp::Renderer::SetRasterize(D3D11_FILL_SOLID, D3D11_CULL_BACK);
}

//ModelManager‚ÌAsset‚©‚ç“Ç‚Ýž‚Ý
void DirectX::Model::GetAsset(std::string name)
{
	Model* asset = ModelManager::GetModel(name).lock().get();
	*this = *asset;
}

void DirectX::Model::SetCulling(bool enable)
{
	IsCulling = enable;
}

void DirectX::Model::SetDepth(bool enable)
{
	IsDepth = enable;
}

void DirectX::Model::SetoffsetMatrix(XMMATRIX matrix)
{
	offsetMatrix = matrix;
}
