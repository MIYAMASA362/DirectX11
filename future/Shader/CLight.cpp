#include"main.h"
#include"renderer.h"
#include"game_object.h"

#include"shader.h"

#include "CLight.h"

CLight::CLight()
{
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(float);
	bd.ByteWidth = sizeof(LIGHT);
	CRenderer::GetDevice()->CreateBuffer(&bd, NULL, &m_Buffer);

	m_Context.Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	m_Context.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Context.Ambient = COLOR(0.25f, 0.25f, 0.25f, 1.0f);
}

CLight::~CLight()
{
	if (m_Buffer) m_Buffer->Release();
}

void CLight::Draw()
{
	CRenderer::GetDeviceContext()->UpdateSubresource(m_Buffer,0,NULL,&m_Context,0,0);
}
