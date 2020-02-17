#include<memory>
#include<map>
#include<vector>

#include"Common.h"
#include"Module\DirectX\DirectX.h"
#include"Module\ECSEngine.h"

#include "Mesh.h"

Mesh::Mesh()
{
	_VertexBuffer = nullptr;
	_IndexBuffer = nullptr;
	
	_VertexArray = nullptr;
	_VertexNum = 0;
	
	_IndexArray = nullptr;
	_IndexNum = 0;
	
	_Surface = nullptr;
	_SurfaceNum = 0;
}

Mesh::~Mesh()
{
	if (_Surface) delete[] _Surface;
	
	if (_IndexArray) delete[] _IndexArray;
	if (_VertexArray) delete[] _VertexArray;
	
	if (_IndexBuffer) _IndexBuffer->Release();
	if (_VertexBuffer) _VertexBuffer->Release();
}

void Mesh::SetVertexBuffer()
{
	D3DRenderer::GetInstance()->SetVertexBuffer(_VertexBuffer, sizeof(VertexType), 0);
}

void Mesh::SetIndexBuffer()
{
	D3DRenderer::GetInstance()->SetIndexBuffer(_IndexBuffer);
}

void Mesh::CreateVertexBuffer()
{
	D3DRenderer::GetInstance()->CreateBuffer(
		D3D11_BIND_VERTEX_BUFFER,
		sizeof(VertexType) * _VertexNum,
		_VertexArray,
		&_VertexBuffer
	);
}

void Mesh::CreateIndexBuffer()
{
	D3DRenderer::GetInstance()->CreateBuffer(
		D3D11_BIND_INDEX_BUFFER,
		sizeof(unsigned short) * _IndexNum,
		_IndexArray,
		&_IndexBuffer
	);
}


//*********************************************************************************************************************
//
//	MeshManager
//
//*********************************************************************************************************************

MeshManager::MeshIndex MeshManager::g_MeshIndex;

//RegisterIndex
//	インテックス登録
//
std::weak_ptr<Mesh> MeshManager::RegisterIndex(std::string name,Mesh * mesh)
{
	return g_MeshIndex.emplace(name,std::shared_ptr<Mesh>(mesh)).first->second;
}

//Release
//	インデックス解放
//
void MeshManager::Release()
{
	g_MeshIndex.clear();
}

std::weak_ptr<Mesh> MeshManager::GetMesh(std::string name)
{
	return g_MeshIndex.at(name);
}

MeshFilter::MeshFilter()
{
}

void MeshFilter::SetMesh(std::weak_ptr<Mesh> mesh)
{
	_mesh = mesh;
	SetIndexNum(mesh.lock()->_IndexNum,0);
}

void MeshFilter::SetIndexNum(unsigned int IndexNum, unsigned int StartNum)
{
	_IndexNum = IndexNum;
	_IndexStartNum = StartNum;
}
