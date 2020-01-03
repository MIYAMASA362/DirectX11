#include"Common.h"

#include"Module\DirectX\DirectX.h"

#include"Module\IMGUI\GUI_ImGui.h"

//ECS
#include"Module\ECSEngine.h"

#include"Module\Texture\texture.h"
#include"Module\Material\Material.h"

#include"Module\Shader\Shader.h"

//Module
#include"Module\Renderer\Renderer.h"
#include"Module\Mesh\Mesh.h"
#include"MeshRender.h"

#include"Module\Transform\Transform.h"


using namespace DirectX;

MeshRender::MeshRender(EntityID OwnerID)
	:
	Renderer3D(OwnerID)
{
	this->OnDebugImGui = [this]() {
		if (ImGui::TreeNode("MeshRender")) 
		{
			bool enable = this->GetEnable();

			ImGui::Checkbox("IsEnable",&enable);
			ImGui::TreePop();

			this->SetEnable(enable);
		}
	};
}

MeshRender::~MeshRender()
{
	_MeshFilter.reset();
}

void MeshRender::Render(XMMATRIX& worldMatrix)
{
	D3DApp::Renderer::SetWorldMatrix(&worldMatrix);

	this->_TextureMaterial->SetResource();

	//シェーダ
	D3DApp::Renderer::GetShader()->SetShader();

	for(unsigned int index = 0; index < _MeshFilter->GetNumMesh(); index++)
	{
		//メッシュの取得
		auto mesh = _MeshFilter->GetMesh(index);

		D3DApp::Renderer::SetWorldMatrix(&worldMatrix);

		//バッファ設定
		mesh.lock()->SetVertexBuffer();
		mesh.lock()->SetIndexBuffer();

		//描画
		D3DApp::Renderer::GetD3DAppDevice()->GetDeviceContext()->IASetPrimitiveTopology(this->_PrimitiveTopology);
		D3DApp::Renderer::GetD3DAppDevice()->GetDeviceContext()->DrawIndexed(mesh.lock()->_IndexNum,0,0);
	}
}
