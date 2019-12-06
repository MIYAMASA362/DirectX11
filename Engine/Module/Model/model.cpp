#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<assert.h>
#include"Common.h"

//DirectX
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

//Engine
#include"Module\ECSEngine.h"

#include"Module\AssetData\AssetData.h"

#include"Module\Material\Material.h"
#include"Module\Texture\texture.h"

//Component Module
#include"Module\Renderer\Renderer.h"

#include"ModelManager.h"

#include"Module\Texture\TextureManager.h"

#include"Module\Mesh\Mesh.h"

#include"model.h"
#include"Module\Model\ModelManager.h"

#include"Module\Shader\Shader.h"

DirectX::Model::Model()
{
	offsetMatrix = XMMatrixIdentity();
}

//描画
void DirectX::Model::Render(XMMATRIX& worldMatrix)
{
	if(IsCulling)
		D3DApp::Renderer::SetRasterize(D3D11_FILL_SOLID,D3D11_CULL_NONE);
	
	worldMatrix = offsetMatrix * worldMatrix;

	D3DApp::Renderer::SetWorldMatrix(&worldMatrix);

	D3DApp::Renderer::SetVertexBuffer(VertexBuffer);
	D3DApp::Renderer::SetIndexBuffer(IndexBuffer);

	for(unsigned short i = 0; i<SubsetNum; i++)
	{
		SubsetArray[i].Material.Material.SetResource();

		D3DApp::Renderer::SetTexture(SubsetArray[i].Material.Texture->GetShaderResourceView());

		D3DApp::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		D3DApp::GetDeviceContext()->DrawIndexed((UINT)SubsetArray[i].IndexNum, (UINT)SubsetArray[i].StartIndex, 0);
	}
	if(IsCulling)
		D3DApp::Renderer::SetRasterize(D3D11_FILL_SOLID, D3D11_CULL_BACK);
}

//ModelManagerのAssetから読み込み
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
