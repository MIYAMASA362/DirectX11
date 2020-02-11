#include "game_object.h"
#include "CLight.h"
#include"shader.h"

#include "input.h"

CLight::CLight()
{

}

void CLight::Init()
{
	m_Viewport.Width = SCREEN_WIDTH;
	m_Viewport.Height = SCREEN_HEIGHT;
	m_Viewport.MinDepth = 0.f;
	m_Viewport.MaxDepth = 1.f;
	m_Viewport.TopLeftX = 0.f;
	m_Viewport.TopLeftY = 0.f;
}

void CLight::Update()
{
#if 0
	if (CInput::GetKeyPress('U')) this->m_Rotation.x += 0.1f;
	if (CInput::GetKeyPress('J')) this->m_Rotation.x -= 0.1f;
	if (CInput::GetKeyPress('H')) this->m_Rotation.y -= 0.1f;
	if (CInput::GetKeyPress('K')) this->m_Rotation.y += 0.1f;
	if (CInput::GetKeyPress('Y')) this->m_Rotation.z -= 0.1f;
	if (CInput::GetKeyPress('I')) this->m_Rotation.z += 0.1f;
#else
	if (CInput::GetKeyPress('U')) this->m_Position.z += 0.1f;
	if (CInput::GetKeyPress('J')) this->m_Position.z -= 0.1f;
	if (CInput::GetKeyPress('H')) this->m_Position.x -= 0.1f;
	if (CInput::GetKeyPress('K')) this->m_Position.x += 0.1f;
	if (CInput::GetKeyPress('Y')) this->m_Position.y += 0.1f;
	if (CInput::GetKeyPress('I')) this->m_Position.y -= 0.1f;
#endif
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

	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.f,m_Viewport.Width / m_Viewport.Height,1.f,1000.f);
}

void CLight::Draw()
{

}

void CLight::UnInit()
{

}

XMFLOAT4X4 CLight::GetViewMatrix()
{
	XMFLOAT4X4 viewMatrixf;
	XMStoreFloat4x4(&viewMatrixf,m_ViewMatrix);
	return viewMatrixf;
}

XMFLOAT4X4 CLight::GetProjectionMatrix()
{
	XMFLOAT4X4 projectionMatrixf;
	XMStoreFloat4x4(&projectionMatrixf,m_ProjectionMatrix);
	return projectionMatrixf;
}

