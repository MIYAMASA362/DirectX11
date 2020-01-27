#include"game_object.h"
#include"renderer.h"

#include "Camera.h"

CCamera::CCamera()
{

}

void CCamera::Init()
{
	m_Viewport.left = 0;
	m_Viewport.top = 0;
	m_Viewport.right = SCREEN_WIDTH;
	m_Viewport.bottom = SCREEN_HEIGHT;
	this->m_Rotation.x = XM_PIDIV2 * 0.5f;

	m_Position = XMFLOAT3(0.0f,2.0f,-2.0f);
	m_Rotation = XMFLOAT3(0.2f,0.0f,0.0f);

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(float);
	bd.ByteWidth = sizeof(CAMERA);

	CRenderer::GetDevice()->CreateBuffer(&bd,NULL,&_CameraBuffer);
}


void CCamera::UnInit()
{

}


void CCamera::Update()
{
	//m_Rotation.y += 0.01f;
}



void CCamera::Draw()
{
	XMMATRIX	m_InvViewMatrix;

	// ビューポート設定
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width = (float)(m_Viewport.right - m_Viewport.left);
	dxViewport.Height = (float)(m_Viewport.bottom - m_Viewport.top);
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = (float)m_Viewport.left;
	dxViewport.TopLeftY = (float)m_Viewport.top;

	CRenderer::GetDeviceContext()->RSSetViewports(1, &dxViewport);

	// ビューマトリクス設定
	m_InvViewMatrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	m_InvViewMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	XMVECTOR det;
	m_ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);

	// プロジェクションマトリクス設定
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);

	_Constant.Position = XMFLOAT4(m_Position.x,m_Position.y,m_Position.z,0.0f);
	CRenderer::GetDeviceContext()->UpdateSubresource(_CameraBuffer,0,NULL,&_Constant,0,0);

	CRenderer::GetDeviceContext()->VSSetConstantBuffers(2,1,&_CameraBuffer);
	CRenderer::GetDeviceContext()->PSSetConstantBuffers(2,1,&_CameraBuffer);
}

