#include"Common.h"

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

//ECS
#include"Module\ECSEngine.h"

#include"Renderer.h"
#include"Module\Transform\Transform.h"

using namespace DirectX;

std::unordered_map<EntityID, std::weak_ptr<Renderer>> Renderer::Index;

void DirectX::Renderer::BeginRender()
{
	auto itr = Index.begin();
	auto end = Index.end();
	while (itr != end) {
		auto renderer = itr->second.lock();
		itr++;
		if (!renderer->m_IsEnable) continue;
		renderer->Render(renderer->transform().lock()->WorldMatrix());
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