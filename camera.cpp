#include"main.h"
#include"Window.h"
#include"DirectXStruct.h"
#include"DirectX.h"
#include"Object.h"
#include"Component.h"
#include"Behaviour.h"
#include"Tag.h"
#include"Transform.h"
#include"GameObject.h"
#include "camera.h"

using namespace DirectX;

std::list<std::shared_ptr<Camera>> Camera::CameraIndex;

Camera::Camera()
:
	viewport({0,0,(long)D3DApp::GetScreenWidth(),(long)D3DApp::GetScreenHeight()}),
	BGColor(Color::gray()),
	priority(1)
{

}

DirectX::Camera::~Camera()
{

}

void Camera::Initialize()
{
	CameraIndex.push_back(std::shared_ptr<Camera>(this));
}

void DirectX::Camera::BeginRun(void (*Draw)(void),void (*Begin)(Color))
{
	for(std::shared_ptr<Camera> camera:CameraIndex)
	{
		Begin(camera._Get()->BGColor);	
		camera._Get()->Run();
		Draw();
	}
}

void DirectX::Camera::Release()
{
	CameraIndex.clear();
}

void Camera::Run()
{
	XMMATRIX	m_ViewMatrix;
	XMMATRIX	m_InvViewMatrix;
	XMMATRIX	m_ProjectionMatrix;

	// ビューポート設定
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width = (float)(viewport.right - viewport.left);
	dxViewport.Height = (float)(viewport.bottom - viewport.top);
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = (float)viewport.left;
	dxViewport.TopLeftY = (float)viewport.top;

	D3DApp::GetDeviceContext()->RSSetViewports(1, &dxViewport);


	// ビューマトリクス設定
	m_InvViewMatrix = gameObject->transform.MatrixRotationRollPitchYaw();
	m_InvViewMatrix *= gameObject->transform.MatrixTranslation();

	XMVECTOR det;
	m_ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);

	D3DApp::Renderer::SetViewMatrix(&m_ViewMatrix);

	// プロジェクションマトリクス設定
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);

	D3DApp::Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

void Camera::SetViewPort(long x, long y, long w, long h)
{
	viewport.left = x;
	viewport.top = y;
	viewport.right = x + w;
	viewport.bottom = y + h;
}