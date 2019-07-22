#include<list>
#include<memory>
#include<Windows.h>
#include<d3d11.h>
#include<DirectXMath.h>

//DirectX
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

//Component
#include"Module\Object\Object.h"
#include"Module\Component\Component.h"
#include"Module\Behaviour\Behaviour.h"

//Component Module
#include"camera.h"
#include"Module\Transform\Transform.h"

//GameObject
#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"


using namespace DirectX;

//--- CameraManager -----------------------------------------------------------

CameraManager* CameraManager::pInstance = nullptr;
std::list<std::weak_ptr<Camera>> CameraManager::CameraIndex;

DirectX::CameraManager::~CameraManager()
{
	CameraIndex.clear();
}

void DirectX::CameraManager::Create()
{
	if (pInstance != nullptr) return;
	pInstance = new CameraManager();
}

void DirectX::CameraManager::Destroy()
{
	if (pInstance != nullptr){
		CameraIndex.clear();
		delete pInstance;
	}
}

void DirectX::CameraManager::IndexSort(Camera * target)
{
	std::weak_ptr<Camera> wPtr = target->gameObject.lock()->GetComponent<Camera>();

	//�z�񒷂�0
	if (CameraIndex.size() == 0) return CameraIndex.push_back(wPtr);

	bool IsInsert = false;

	//�z��
	for (auto itr = CameraIndex.begin(); itr != CameraIndex.end();)
	{
		Camera* camera = itr->lock().get();
		//�폜
		if (camera == target)
		{
			itr->reset();
			itr = CameraIndex.erase(itr);
			continue;
		}

		//�}��
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

void DirectX::CameraManager::RemoveCamera(Camera * camera)
{
	for (auto itr = CameraIndex.begin(); itr != CameraIndex.end(); itr++)
	{
		if (itr->lock().get() != camera) continue;
		CameraIndex.erase(itr);
		break;
	}
}

void DirectX::CameraManager::SetRender(void(*Draw)(void), void(*Begin)(void))
{
	auto itr = CameraIndex.begin();
	while (itr != CameraIndex.end())
	{
		//���g�p�g���폜
		if (itr->expired()) {
			itr = CameraIndex.erase(itr);
			continue;
		}
		//�J�����ݒ�
		Camera* camera = itr->lock().get();
		itr++;

		if (!camera->gameObject.lock()->GetActive())continue;
		if (!camera->IsEnable)continue;

		Begin();
		camera->Run();
		Draw();
	}
}

void DirectX::CameraManager::Release()
{
	CameraIndex.clear();
}

//--- Camera ------------------------------------------------------------------

DirectX::Camera::Camera()
:
	viewport({0,0,(long)D3DApp::GetScreenWidth(),(long)D3DApp::GetScreenHeight()}),
	priority(1)
{

}

DirectX::Camera::~Camera()
{

}

void DirectX::Camera::OnDestroy()
{
	CameraManager::RemoveCamera(this);
}

//�`��
void DirectX::Camera::Run()
{
	XMMATRIX	m_ViewMatrix;
	XMMATRIX	m_InvViewMatrix;
	XMMATRIX	m_ProjectionMatrix;

	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT dxViewport;
	dxViewport.Width = (float)(viewport.right - viewport.left);
	dxViewport.Height = (float)(viewport.bottom - viewport.top);
	dxViewport.MinDepth = 0.0f;
	dxViewport.MaxDepth = 1.0f;
	dxViewport.TopLeftX = (float)viewport.left;
	dxViewport.TopLeftY = (float)viewport.top;

	D3DApp::GetDeviceContext()->RSSetViewports(1, &dxViewport);

	// �r���[�}�g���N�X�ݒ�
	m_InvViewMatrix = gameObject.lock()->transform->WorldMatrix();

	XMVECTOR det;
	m_ViewMatrix = XMMatrixInverse(&det, m_InvViewMatrix);

	D3DApp::Renderer::SetViewMatrix(&m_ViewMatrix);

	// �v���W�F�N�V�����}�g���N�X�ݒ�
	m_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, dxViewport.Width / dxViewport.Height, 1.0f, 1000.0f);

	D3DApp::Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
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

void DirectX::Camera::SetPriority(int priority)
{
	this->priority = priority;
	CameraManager::IndexSort(this);
}

void DirectX::Camera::Finalize()
{
	CameraManager::RemoveCamera(this);
}
