#include"main.h"
#include"Window.h"
#include"DirectXStruct.h"
#include"DirectX.h"
#include"Object.h"
#include"Component.h"
#include"Behaviour.h"
#include"Tag.h"
#include"Transform.h"
#include"Renderer.h"
#include"GameObject.h"
#include"camera.h"

using namespace DirectX;

std::list<std::weak_ptr<Camera>> Camera::CameraIndex;

Camera::Camera()
:
	viewport({0,0,(long)D3DApp::GetScreenWidth(),(long)D3DApp::GetScreenHeight()}),
	priority(1)
{

}

DirectX::Camera::~Camera()
{

}

//整列
void DirectX::Camera::IndexSort(Camera* target)
{
	std::weak_ptr<Camera> wPtr = target->gameObject.lock()->GetComponent<Camera>();

	//配列長が0
	if(CameraIndex.size() == 0) return CameraIndex.push_back(wPtr);

	bool IsInsert = false;

	//配列
	for(auto itr = CameraIndex.begin(); itr != CameraIndex.end();)
	{
		Camera* camera = itr->lock().get();
		//削除
		if (camera == target)
		{
			itr->reset();
			itr = CameraIndex.erase(itr);
			continue;
		}

		//挿入
		if (!IsInsert)
			if (camera->priority >= target->priority)
			{
				IsInsert = true;
				if (itr == CameraIndex.begin())
					CameraIndex.push_front(wPtr);
				else
					CameraIndex.insert(itr, wPtr);
			}
		itr++;
	}

	if (!IsInsert) CameraIndex.push_back(wPtr);
	return;
}

void Camera::Initialize()
{
	IndexSort(this);
}

void DirectX::Camera::OnDestroy()
{
	for(auto itr = CameraIndex.begin(); itr != CameraIndex.end(); itr++)
	{
		if (itr->lock().get() != this) continue;
		CameraIndex.erase(itr);
		break;
	}
}

//描画開始
void DirectX::Camera::BeginRun(void (*Draw)(void),void (*Begin)(void))
{
	auto itr = CameraIndex.begin();
	while (itr != CameraIndex.end())
	{
		if (itr->expired()){
			itr = CameraIndex.erase(itr);
			continue;
		}
		Camera* camera = itr->lock().get();
		itr++;

		if (!camera->gameObject.lock()->IsActive)continue;
		if (!camera->IsEnable)continue;

		Begin();
		camera->Run();
		Draw();
	}
}

void DirectX::Camera::Release()
{
	CameraIndex.clear();
}

//描画
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
	m_InvViewMatrix = gameObject.lock()->transform->MatrixQuaternion();
	m_InvViewMatrix *= gameObject.lock()->transform->MatrixTranslation();

	XMVECTOR det;
	m_ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);

	D3DApp::Renderer::SetViewMatrix(&m_ViewMatrix);

	// プロジェクションマトリクス設定
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);

	D3DApp::Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

void Camera::SetViewPort(float x, float y, float w, float h)
{
	x = min(x, 1.0f);
	y = min(y, 1.0f);
	w = min(w, 1.0f);
	h = min(h, 1.0f);

	viewport.left	= (long)(x == 0.0f ? 0 : D3DApp::GetScreenWidth() *x);
	viewport.right	= (long)(w == 0.0f ? 0 : D3DApp::GetScreenWidth() *w);
	viewport.top	= (long)(y == 0.0f ? 0 : D3DApp::GetScreenHeight()*y);
	viewport.bottom = (long)(h == 0.0f ? 0 : D3DApp::GetScreenHeight()*h);
}

void DirectX::Camera::SetPriority(int priority)
{
	this->priority = priority;
	IndexSort(this);
}
