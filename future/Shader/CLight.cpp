#include "game_object.h"
#include "CLight.h"
#include"shader.h"

#include "input.h"

CLight::CLight()
{

}

void CLight::Init()
{
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(float);
	bd.ByteWidth = sizeof(LIGHT);
	
	CRenderer::GetDevice()->CreateBuffer(&bd,NULL,&m_LightBuffer);
}

void CLight::Update()
{
	if (CInput::GetKeyPress('U')) this->m_Rotation.x += 0.1f;
	if (CInput::GetKeyPress('J')) this->m_Rotation.x -= 0.1f;
	if (CInput::GetKeyPress('H')) this->m_Rotation.y -= 0.1f;
	if (CInput::GetKeyPress('K')) this->m_Rotation.y += 0.1f;
	if (CInput::GetKeyPress('Y')) this->m_Rotation.z -= 0.1f;
	if (CInput::GetKeyPress('I')) this->m_Rotation.z += 0.1f;
}

void CLight::DrawShadow()
{
	XMMATRIX rotMtx;
	rotMtx = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	XMMATRIX invViewMatrix;

	invViewMatrix = rotMtx;
	invViewMatrix *= XMMatrixTranslation(m_Position.x,m_Position.y,m_Position.z);

	XMVECTOR det;
	m_ViewMatrix = XMMatrixInverse(&det,invViewMatrix);

	XMVECTOR dir = XMVectorSet(0.f,-1.f,0.f,0.f);
	dir = XMVector3TransformCoord(dir,rotMtx);
	m_Light.Direction = XMFLOAT4(dir.m128_f32[0],dir.m128_f32[1],dir.m128_f32[2],dir.m128_f32[3]);
}

void CLight::Draw()
{

}

void CLight::UnInit()
{
	if (m_LightBuffer) m_LightBuffer->Release();
}

void CLight::UpdateSubResource()
{
	CRenderer::GetDeviceContext()->UpdateSubresource(m_LightBuffer,0,NULL,&m_Light,0,0);
}

void CLight::SetViewMatrix(XMFLOAT4X4 * ViewMatrix)
{
	m_Light.ViewMatrix = Transpose(ViewMatrix);
}

void CLight::SetProjectionMatrix(XMFLOAT4X4 * ProjectionMatrix)
{
	m_Light.ProjMatrix = Transpose(ProjectionMatrix);
}
