#include<memory>
#include<string>
#include<list>
#include<map>

#include<d3d11.h>
#include<DirectXMath.h>

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

#include"Module\Object\Object.h"
#include"Module\Component\IComponent.h"
#include"Module\Component\ComponentManager.h"
#include"Module\Component\Component.h"
#include"Module\Mesh\Mesh.h"
#include"MeshRender.h"

#include"Module\Transform\Transform.h"

using namespace DirectX;

DirectX::MeshRender::MeshRender()
	:
	Component("MeshRender")
{

}

DirectX::MeshRender::~MeshRender()
{
	mesh.reset();
}

void DirectX::MeshRender::OnComponent()
{

}

void DirectX::MeshRender::Render()
{
	if (!this->m_IsEnable)return;
	mesh->Render(ComponentManager::GetComponent<Transform>(m_OwnerId).lock()->WorldMatrix());
}