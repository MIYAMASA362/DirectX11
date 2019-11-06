#include"Common.h"

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

//ECS
#include"Module\ECSEngine.h"

#include"Renderer.h"
#include"Module\Transform\Transform.h"

using namespace DirectX;

void DirectX::Renderer::BeginRender()
{
	auto Index = ComponentManager::GetOrCreateComponentIndex(Renderer::TypeID).lock();
	for(auto object:*Index){
		auto renderer = std::dynamic_pointer_cast<Renderer>(object.second.lock());
		if (!renderer->GetEnable()) continue;
		renderer->Render(renderer->transform()->WorldMatrix());
	}
}

DirectX::Renderer::Renderer(EntityID OwnerID)
:
	Component(OwnerID)
{
	
}

void DirectX::Renderer::SetEnable(bool enable)
{
	this->m_IsEnable = enable;
}

bool DirectX::Renderer::GetEnable()
{
	return this->m_IsEnable;
}