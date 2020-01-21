#define INCLUDE_CEREAL
#include"Common.h"

#include"Module\DirectX\DirectX.h"

#include"Module\IMGUI\GUI_ImGui.h"

//ECS
#include"Module\ECSEngine.h"
#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"

//Module
#include"Module\Texture\texture.h"
#include"Module\Material\Material.h"

#include"Module\Shader\Shader.h"

#include"Module\Renderer\Renderer.h"
#include"Module\Mesh\Mesh.h"

#include"Module\Transform\Transform.h"

#include"Module\Texture\TextureManager.h"
#include"Module\Model\model.h"
#include"MeshRender.h"


using namespace DirectX;



//*********************************************************************************************************************
//
//	MeshRenderer
//
//*********************************************************************************************************************

//MeshRender
//	コンストラクタ
//
MeshRender::MeshRender(EntityID OwnerID)
	:
	Renderer(OwnerID)
{
}

//~MeshRender
//	デストラクタ
//
MeshRender::~MeshRender()
{
	
}

//Render
//	描画
//
void MeshRender::Render(XMMATRIX& worldMatrix)
{
	if (_meshfilter.expired())
		_meshfilter = this->gameObject()->GetComponent<MeshFilter>();

	D3DRenderer::GetInstance()->SetWorldMatrix(&worldMatrix);
	auto filter = _meshfilter.lock();
	auto mesh = filter->GetMesh().lock();

	mesh->SetVertexBuffer();
	mesh->SetIndexBuffer();

	_Material.SetResource();

	D3DRenderer::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(mesh->_PrimitiveTopology);
	D3DRenderer::GetInstance()->GetDeviceContext()->DrawIndexed(filter->GetIndexNum(),filter->GetIndesStartNum(), 0);
}

//GetMeshFilter
//	メッシュフィルターの取得
//
std::shared_ptr<MeshFilter> MeshRender::GetMeshFilter()
{
	if (!_meshfilter.expired())
		_meshfilter = gameObject()->GetComponent<MeshFilter>();
	return _meshfilter.lock();
}

void MeshRender::OnDebugImGui()
{
	bool enable = this->GetEnable();

	ImGui::Checkbox("IsEnable", &enable);
	ImGui::TreePop();

	this->SetEnable(enable);
}
