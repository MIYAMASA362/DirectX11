#include<memory>
#include<string>
#include<list>
#include<map>

#include<d3d11.h>
#include<DirectXMath.h>

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
	Renderer(OwnerID,"MeshRender")
{

}

DirectX::MeshRender::~MeshRender()
{
	mesh.reset();
}

void DirectX::MeshRender::OnComponent()
{

}

void DirectX::MeshRender::Render(XMMATRIX& worldMatrix)
{
	mesh->Render(worldMatrix);
}

void MeshRender::DebugImGui()
{
	if (ImGui::TreeNode("MeshRender")) {
		ImGui::TreePop();
	}
}