#include"game_object.h"

#include"shader.h"
#include"texture.h"

#include "CCube.h"

CCube::CCube()
{
	const int vertexNum = 4;
	VERTEX_3D vertex[vertexNum];

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &_VertexBuffer);

	_Shader = new CShader();
	_Shader->Init("Shader3DTestVS.cso", "Shader3DTestPS.cso");

	_Texture = new CTexture();
	_Texture->Load("data/TEXTURE/field004.tga");
}

void CCube::Init()
{
}

void CCube::Update()
{
}

void CCube::Draw()
{
}

void CCube::UnInit()
{
}
