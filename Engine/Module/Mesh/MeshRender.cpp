#include"Common.h"

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"Module\IMGUI\GUI_ImGui.h"

//ECS
#include"Module\ECSEngine.h"

//Module
#include"Module\Renderer\Renderer.h"
#include"Module\Mesh\Mesh.h"
#include"MeshRender.h"

#include"Module\Transform\Transform.h"

using namespace DirectX;

DirectX::MeshRender::MeshRender(EntityID OwnerID)
	:
	Renderer(OwnerID)
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

DirectX::MeshRender::~MeshRender()
{
	mesh.reset();
}

void DirectX::MeshRender::Render(XMMATRIX& worldMatrix)
{
	mesh->Render(worldMatrix);
}