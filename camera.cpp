
#include "main.h"
#include "camera.h"
#include"DirectX.h"




void CCamera::Initialize()
{
	m_Viewport.left = 0;
	m_Viewport.top = 0;
	m_Viewport.right = D3DApp::GetScreenWidth();
	m_Viewport.bottom = D3DApp::GetScreenHeight();
}


void CCamera::Finalize()
{
	
}


void CCamera::Update()
{
	gameObject->transform.rotation.y += XM_PI / 180 * 0.5;
}



void CCamera::Render()
{
	XMMATRIX	m_ViewMatrix;
	XMMATRIX	m_InvViewMatrix;
	XMMATRIX	m_ProjectionMatrix;

	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width = (float)(m_Viewport.right - m_Viewport.left);
	dxViewport.Height = (float)(m_Viewport.bottom - m_Viewport.top);
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = (float)m_Viewport.left;
	dxViewport.TopLeftY = (float)m_Viewport.top;

	D3DApp::GetDeviceContext()->RSSetViewports(1, &dxViewport);


	// �r���[�}�g���N�X�ݒ�
	m_InvViewMatrix = gameObject->transform.MatrixRotationRollPitchYaw();
	m_InvViewMatrix *= gameObject->transform.MatrixTranslation();

	XMVECTOR det;
	m_ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);

	D3DApp::Renderer::SetViewMatrix(&m_ViewMatrix);



	// �v���W�F�N�V�����}�g���N�X�ݒ�
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);

	D3DApp::Renderer::SetProjectionMatrix(&m_ProjectionMatrix);



}

