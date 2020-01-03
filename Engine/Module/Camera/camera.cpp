#include<algorithm>

#include"Common.h"
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

//*********************************************************************************************************************
//
//	Camera
//
//*********************************************************************************************************************


//pActiveCamera
Camera* Camera::pActiveCamera = nullptr;

//CameraSortIndex
std::vector<Camera*> Camera::CameraSortIndex;



//Camera
//	�R���X�g���N�^
//
Camera::Camera(EntityID OwnerID)
	:
	Behaviour(OwnerID),
	_Priority(1),
	_ViewMatrix(XMMatrixIdentity()),
	_ProjectionMatrix(XMMatrixIdentity())
{
	SetViewPort(0, 0, 1, 1);	//ViewPort�ݒ�

	this->RegisterIndex();		//ComponentIndex�֒ǉ�
	this->RegisterSortIndex(this);

	if (pActiveCamera == nullptr)
		pActiveCamera = this;

	//ImGui�\��
	this->OnDebugImGui = [this]() {
		if (ImGui::TreeNode("Camera"))
		{
			ImGui::Text(("ID:" + std::to_string(this->GetInstanceID())).c_str());
			ImGui::Text(("Priority:" + std::to_string(this->_Priority)).c_str());

			ImGui::TreePop();
		}
	};
}

//~Camera
//	�f�X�g���N�^
//
Camera::~Camera()
{
	RemoveSortIndex(this);
}


//Render
//	�`��
//
void Camera::Render(void(*Draw)(void), void(*Begin)(void))
{
	//Component���Q��
	for (auto camera : CameraSortIndex)
	{
		//�L��
		if (!camera->gameObject()->GetActive()) continue;
		if (!camera->GetEnable()) continue;

		//�A�N�e�B�u�ȃJ�����ɐݒ�
		pActiveCamera = camera;

		Begin();
		camera->Run();
		Draw();
	}
}


//ScreenToWorldPosition
//	�������X�N���[�����W�ɕϊ�
//
Vector3 Camera::ScreenToWorldPosition(Vector3 position)
{
	XMFLOAT2 MousePos;
	MousePos.x = Input::Mouse::GetMouseX();
	MousePos.y = Input::Mouse::GetMouseY();

	RECT rect;
	GetWindowRect(D3DApp::Renderer::GetD3DAppDevice()->GetWindow(), &rect);

	const XMMATRIX projection = pActiveCamera->GetProjectionMatrix();
	const XMMATRIX ViewMatrix = pActiveCamera->GetViewMatrix();
	const XMMATRIX WorldMatrix = pActiveCamera->transform()->WorldMatrix();

	Vector3 screenRay;
	//2.0f�͉�ʒ��S�Ɉړ������邽��
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

	screenRay = XMVector3TransformCoord(screenRay, pActiveCamera->transform()->MatrixQuaternion());

	return pActiveCamera->transform()->position() + screenRay*10.0f;
}


//RegisterSortIndex
//	CameraSortIndex�ɒǉ�����
//
void Camera::RegisterSortIndex(Camera* target)
{
	RemoveSortIndex(target);
	CameraSortIndex.push_back(target);//�ǉ�
	std::sort(CameraSortIndex.begin(), CameraSortIndex.end(), [](const Camera* a, const Camera* b) { return a->GetPriority() > b->GetPriority(); });	//�\�[�g
	return;
}

//RemoveSoetIndex
//	CameraSortIndex����폜����
//
void Camera::RemoveSortIndex(Camera * target)
{
	auto end = std::remove_if(CameraSortIndex.begin(), CameraSortIndex.end(), [target](const Camera* camera) { return camera == target; });
	CameraSortIndex.erase(end, CameraSortIndex.end());
}

//SetViewPort
//	�r���[�|�[�g�ݒ�
//
void Camera::SetViewPort(float x, float y, float w, float h)
{
	x = min(x, 1.0f);
	y = min(y, 1.0f);
	w = min(w, 1.0f);
	h = min(h, 1.0f);

	RECT rect;
	GetWindowRect(D3DApp::Renderer::GetD3DAppDevice()->GetWindow(), &rect);
	float width = rect.right - rect.left;
	float height = rect.bottom - rect.top;

	rect.left = (long)(x == 0.0f ? 0 : width *x);
	rect.right = (long)(w == 0.0f ? 0 : width *w);
	rect.top = (long)(y == 0.0f ? 0 : height*y);
	rect.bottom = (long)(h == 0.0f ? 0 : height*h);

	_Viewport.TopLeftX = rect.left;
	_Viewport.TopLeftY = rect.top;
	_Viewport.Width = rect.right - rect.left;
	_Viewport.Height = rect.bottom - rect.top;
	_Viewport.MaxDepth = 1.0f;
	_Viewport.MinDepth = 0.0f;

}

//SetPriority
//	Camera�`��D��x�ݒ�
//
void Camera::SetPriority(int priority)
{
	this->_Priority = priority;
	Camera::RegisterSortIndex(this);
}

//Run
//	�`����s
//
void Camera::Run()
{
	XMMATRIX	ViewMatrix;
	XMMATRIX	InvViewMatrix;

	D3DApp::Renderer::GetD3DAppDevice()->GetDeviceContext()->RSSetViewports(1, &_Viewport);

	// �r���[�}�g���N�X�ݒ�
	{
		InvViewMatrix = this->transform()->WorldMatrix();

		XMVECTOR det;
		ViewMatrix = XMMatrixInverse(&det, InvViewMatrix);

		this->_ViewMatrix = this->transform()->MatrixScaling() * this->transform()->MatrixQuaternion();

		D3DApp::Renderer::SetViewMatrix(&ViewMatrix);
	}

	// �v���W�F�N�V�����}�g���N�X�ݒ�
	{
		this->_ProjectionMatrix = XMMatrixPerspectiveFovLH(1.0f, _Viewport.Width / _Viewport.Height, 1.0f, 1000.0f);

		D3DApp::Renderer::SetProjectionMatrix(&_ProjectionMatrix);
	}
}

//GetVisibility
//	������J�����O
//
bool Camera::GetVisibility(Vector3 position)
{
	XMVECTOR viewPos, projPos;
	XMFLOAT3 projPosF;

	viewPos = XMVector3TransformCoord(position, _ViewMatrix);
	projPos = XMVector3TransformCoord(viewPos, _ProjectionMatrix);
	XMStoreFloat3(&projPosF, projPos);

	//��ʓ���
	if (-1.0f < projPosF.x && projPosF.x < 1.0f &&
		-1.0f < projPosF.y && projPosF.y < 1.0f &&
		0.0f < projPosF.z && projPosF.z < 1.0f)
	{
		return true;
	}
	return false;
}




//*********************************************************************************************************************
//
//	Editor�p
//
//*********************************************************************************************************************

//EditorWindow
//	�G�f�B�^�\��
//
void Camera::EditorWindow()
{
	ImGui::Begin("Camera : SortIndex");
	for (auto camera : CameraSortIndex)
	{
		//GameObject���Őݒ�
		ImGui::Text((std::to_string(camera->_Priority) + " : " + camera->gameObject()->GetName()).c_str());
	}
	ImGui::End();
}
