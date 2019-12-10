#include"Common.h"

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

#include"Module\Input\Input.h"

#include"Module\Shader\Shader.h"

using namespace DirectX;

//--- Camera ------------------------------------------------------------------
Camera* Camera::pActiveCamera = nullptr;
std::list<std::weak_ptr<Camera>> Camera::CameraIndex;

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

		if (!camera->gameObject()->GetActive()) continue;
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

Vector3 DirectX::Camera::ScreenToWorldPosition(Vector3 position)
{
	XMFLOAT2 MousePos;
	MousePos.x = Input::Mouse::GetMouseX();
	MousePos.y = Input::Mouse::GetMouseY();

	RECT rect;
	GetWindowRect(D3DApp::GetWindow(),&rect);
	
	const XMMATRIX projection = pActiveCamera->GetProjectionMatrix();
	const XMMATRIX ViewMatrix = pActiveCamera->GetViewMatrix();
	const XMMATRIX WorldMatrix = pActiveCamera->transform()->WorldMatrix();

	Vector3 screenRay;
	//2.0fは画面中心に移動させるため
	screenRay.x = ((2.0f * MousePos.x / (rect.right - rect.left)) - 1.0f) / projection.r[0].m128_f32[0];
	screenRay.y = -((2.0f * MousePos.y / (rect.bottom - rect.top)) - 1.0f) / projection.r[1].m128_f32[1];
	screenRay.z = 1.0f;

	XMMATRIX worldView = WorldMatrix * ViewMatrix;
	XMMATRIX InvWorldView = XMMatrixInverse(NULL, worldView);

	Vector3 rayDirection;
	rayDirection.x = screenRay.x * InvWorldView.r[0].m128_f32[0] + screenRay.y * InvWorldView.r[1].m128_f32[0] + screenRay.z * InvWorldView.r[2].m128_f32[0];
	rayDirection.y = screenRay.x * InvWorldView.r[0].m128_f32[1] + screenRay.y * InvWorldView.r[1].m128_f32[1] + screenRay.z * InvWorldView.r[2].m128_f32[1];
	rayDirection.z = screenRay.x * InvWorldView.r[0].m128_f32[2] + screenRay.y * InvWorldView.r[1].m128_f32[2] + screenRay.z * InvWorldView.r[2].m128_f32[2];
	rayDirection = rayDirection.normalize();

	screenRay = XMVector3TransformCoord(screenRay,pActiveCamera->transform()->MatrixQuaternion());

	return pActiveCamera->transform()->position() + screenRay*10.0f;
}

void Camera::IndexSort(Camera* target)
{
	std::weak_ptr<Camera> wPtr = Camera::GetComponent(target->GetOwnerID());

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
	Behaviour(OwnerID),
	viewport({0,0,(long)D3DApp::GetScreenWidth(),(long)D3DApp::GetScreenHeight()}),
	priority(1)
{
	this->m_ProjectionMatrix = XMMatrixIdentity();
	this->m_ViewMatrix = XMMatrixIdentity();

	if (pActiveCamera == nullptr)
		pActiveCamera = this;

	this->OnDebugImGui = [this]() {
		if (ImGui::TreeNode("Camera"))
		{
			ImGui::Text(("ID:" + std::to_string(this->GetInstanceID())).c_str());
			ImGui::Text(("Priority:" + std::to_string(this->priority)).c_str());

			ImGui::TreePop();
		}
	};
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

XMMATRIX DirectX::Camera::GetProjectionMatrix()
{
	return this->m_ProjectionMatrix;
}

//描画
void Camera::Run()
{
	XMMATRIX	ViewMatrix;
	XMMATRIX	m_InvViewMatrix;

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
	{
		m_InvViewMatrix = this->transform()->WorldMatrix();

		XMVECTOR det;
		ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);

		this->m_ViewMatrix = this->transform()->MatrixScaling() * this->transform()->MatrixQuaternion();

		D3DApp::Renderer::SetViewMatrix(&ViewMatrix);
	}

	// プロジェクションマトリクス設定
	{
		this->m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);

		D3DApp::Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
	}
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

//視錐台カリング
bool DirectX::Camera::GetVisibility(Vector3 position)
{
	XMVECTOR viewPos,projPos;
	XMFLOAT3 projPosF;

	viewPos = XMVector3TransformCoord(position,m_ViewMatrix);
	projPos = XMVector3TransformCoord(viewPos,m_ProjectionMatrix);
	XMStoreFloat3(&projPosF,projPos);

	//画面内か
	if (-1.0f < projPosF.x && projPosF.x < 1.0f &&
		-1.0f < projPosF.y && projPosF.y < 1.0f &&
		 0.0f < projPosF.z && projPosF.z < 1.0f)
	{
		return true;
	}
	return false;
}
