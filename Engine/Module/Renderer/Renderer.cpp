#include<list>
#include<memory>
#include<map>

#include<d3d11.h>
#include<DirectXMath.h>

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

//ECS
#include"Module\ECSEngine.h"

#include"Renderer.h"
#include"Module\Transform\Transform.h"

using namespace DirectX;

DirectX::Renderer::Renderer()
:
	Component("Renderer")
{
}

void DirectX::Renderer::Run()
{
	if (!this->m_IsEnable)return;
	this->Render(ComponentManager::GetComponent<Transform>(this->m_OwnerId).lock()->WorldMatrix());
}


void DirectX::Renderer::OnComponent()
{
};