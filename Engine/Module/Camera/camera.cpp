#include<list>
#include<vector>
#include<memory>
#include<map>
#include<string>
#include<Windows.h>
#include<d3d11.h>
#include<DirectXMath.h>

//DirectX
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

//Component
#include"Module\ECSEngine.h"

//Component Module
#include"camera.h"
#include"Module\Transform\Transform.h"

//GameObject
#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"

#include"Module\IMGUI\GUI_ImGui.h"

using namespace DirectX;

//--- Camera ------------------------------------------------------------------
Camera* Camera::pActiveCamera = nullptr;
std::list<std::weak_ptr<Camera>> Camera::CameraIndex;
std::map<EntityID, std::weak_ptr<Camera>> Camera::ComponentIndex;

//--- static method -------------------------------------------------
void Camera::Render(void(*Draw)(void), void(*Begin)(void))
{
	auto itr = CameraIndex.begin();
	while (itr != CameraIndex.end())
	{
		//未使用枠を削除
		if (itr->expired()) {
			itr = CameraIndex.erase(itr);
			continue;
		}
		//カメラ設定
		Camera* camera = itr->lock().get();
		itr++;

		if (!camera->gameObject().lock()->GetActive()) continue;
		if (!camera->m_IsEnable) continue;
		pActiveCamera = camera;

		Begin();
		camera->Run();
		Draw();
	}
}

Camera* Camera::GetActiveCamera()
{
	return pActiveCamera;
}

void Camera::IndexSort(Camera* target)
{
	std::weak_ptr<Camera> wPtr = target->gameObject().lock()->GetComponent<Camera>();

	//配列長が0
	if (CameraIndex.size() == 0) return CameraIndex.push_back(wPtr);

	bool IsInsert = false;

	//配列
	for (auto itr = CameraIndex.begin(); itr != CameraIndex.end();) {
		Camera* camera = itr->lock().get();
		//削除(重複を防ぐ)
		if (camera == target)
		{
			itr->reset();
			itr = CameraIndex.erase(itr);
			continue;
		}

		//挿入
		if (!IsInsert)
			if (camera->priority >= target->priority) {
				IsInsert = true;
				if (itr == CameraIndex.begin())
					CameraIndex.push_front(wPtr);
				else
					CameraIndex.insert(itr, wPtr);
			}
		itr++;
	}

	if (!IsInsert) CameraIndex.push_back(wPtr);	//末尾
	return;
}

//--- Camera --------------------------------------------------------

Camera::Camera(EntityID OwnerID)
:
	Behaviour(OwnerID,"Camera"),
	viewport({0,0,(long)D3DApp::GetScreenWidth(),(long)D3DApp::GetScreenHeight()}),
	priority(1)
{

}

Camera::~Camera()
{
}

void DirectX::Camera::SetViewPort(float x, float y, float w, float h)
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

void Camera::SetPriority(int priority)
{
	this->priority = priority;
	Camera::IndexSort(this);
}

XMMATRIX Camera::GetViewMatrix()
{
	return this->m_ViewMatrix;
}

//描画
void Camera::Run()
{
	XMMATRIX	ViewMatrix;
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
	m_InvViewMatrix = this->transform().lock()->WorldMatrix();

	XMVECTOR det;
	ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);

	this->m_ViewMatrix = this->transform().lock()->MatrixScaling() * this->transform().lock()->MatrixQuaternion();

	D3DApp::Renderer::SetViewMatrix(&ViewMatrix);

	// プロジェクションマトリクス設定
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);

	D3DApp::Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

void Camera::OnComponent()
{
	Camera::IndexSort(this);
}

void Camera::OnDestroy()
{
	for (auto itr = CameraIndex.begin(); itr != CameraIndex.end(); itr++)
	{
		auto camera = itr->lock().get();
		if (camera != this) continue;
		CameraIndex.erase(itr);
		break;
	}
}

void DirectX::Camera::DebugImGui()
{
	if (ImGui::TreeNode((std::string("Camera") + std::to_string(m_OwnerId)).c_str()))
	{
		ImGui::TreePop();
	}
}