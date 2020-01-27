#include"game_object.h"
#include"renderer.h"

#include"texture.h"
#include"Camera.h"

#include "Field.h"

#include"input.h"

CField::CField()
{
	VERTEX_3D_NORMAL vertex[4];
	vertex[0].Position	= XMFLOAT3(-5.0f, 0.0f, 5.0f);

	vertex[0].Normal		= XMFLOAT3(0.0f,1.0f,0.0f);
	vertex[0].Binormal	= XMFLOAT3(0.0f,0.0f,1.0f);
	vertex[0].Tangent	= XMFLOAT3(1.0f,0.0f,0.0f);

	vertex[0].Diffuse	= XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	vertex[0].TexCoord	= XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position	= XMFLOAT3(-5.0f, 0.0f, -5.0f);

	vertex[1].Normal		= XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Binormal	= XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertex[1].Tangent	= XMFLOAT3(1.0f, 0.0f, 0.0f);

	vertex[1].Diffuse	= XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord	= XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position	= XMFLOAT3(5.0f, 0.0f, 5.0f);

	vertex[2].Normal		= XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Binormal	= XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertex[2].Tangent	= XMFLOAT3(1.0f, 0.0f, 0.0f);

	vertex[2].Diffuse	= XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	vertex[2].TexCoord	= XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position	= XMFLOAT3(5.0f, 0.0f, -5.0f);

	vertex[3].Normal		= XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Binormal	= XMFLOAT3(0.0f, 0.0f, 1.0f);
	vertex[3].Tangent	= XMFLOAT3(1.0f, 0.0f, 0.0f);

	vertex[3].Diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord	= XMFLOAT2(1.0f, 1.0f);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D_NORMAL) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &_VertexBuffer);

	_Shader = new CShaderNormal();
	_Shader->Init("shader3DNormalMappingVS.cso","shader3DNormalMappingPS.cso");

	_Texture = new CTexture[3];
	_Texture[0].Load("data/TEXTURE/Rock_Normal.tga");
	_Texture[1].Load("data/TEXTURE/Rock_Displacement.tga");
	_Texture[2].Load("data/TEXTURE/field004.tga");
}

void CField::Init()
{
	m_Scale = XMFLOAT3(1.0f,1.0f,1.0f);
}

void CField::Update()
{
	//m_Rotation.x += 0.01f;
	if (CInput::GetKeyPress('W'))
		this->m_Position.z += 0.1f;
	if (CInput::GetKeyPress('S'))
		this->m_Position.z -= 0.1f;
	if (CInput::GetKeyPress('A'))
		this->m_Position.x -= 0.1f;
	if (CInput::GetKeyPress('D'))
		this->m_Position.x += 0.1f;

	if (CInput::GetKeyPress('Q'))
		this->m_Rotation.x += 0.1f;
	if (CInput::GetKeyPress('E'))
		this->m_Rotation.x -= 0.1f;
}

void CField::Draw()
{
	UINT stride = sizeof(VERTEX_3D_NORMAL);
	UINT offset = 0;

	CRenderer::GetDeviceContext()->IASetVertexBuffers(0,1,&_VertexBuffer,&stride,&offset);

	ID3D11ShaderResourceView* Textures[]
	{
		_Texture[0].GetShaderResourceView(),
		_Texture[1].GetShaderResourceView(),
		_Texture[2].GetShaderResourceView()
	};
	CRenderer::SetTexture(Textures, 0, 3);

	XMMATRIX world;
	world = XMMatrixIdentity();
	world = XMMatrixScaling(m_Scale.x,m_Scale.y,m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x,m_Rotation.y,m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x,m_Position.y,m_Position.z);

	XMFLOAT4X4 worldf;
	DirectX::XMStoreFloat4x4(&worldf,world);
	_Shader->SetWorldMatrix(&worldf);

	XMFLOAT4X4 viewf;
	DirectX::XMStoreFloat4x4(&viewf, _Camera->GetViewMatrix());
	_Shader->SetViewMatrix(&viewf);
	
	XMFLOAT4X4 projf;
	DirectX::XMStoreFloat4x4(&projf,_Camera->GetProjectionMatrix());
	_Shader->SetProjectionMatrix(&projf);

	_Shader->Set();

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->Draw(4,0);
}

void CField::UnInit()
{
	_Shader->Uninit();
	delete _Shader;

	delete[] _Texture;
	_VertexBuffer->Release();
}
