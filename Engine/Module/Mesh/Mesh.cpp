#include<memory>
#include<map>
#include<vector>

#include"Module\DirectX\DirectX.h"
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