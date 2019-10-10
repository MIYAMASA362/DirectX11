#include"Common.h"

#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

//ECS
#include"Module\ECSEngine.h"

#include"Renderer.h"
#include"Module\Transform\Transform.h"

using namespace DirectX;
std::unordered_map<EntityID, std::weak_ptr<Renderer>> Renderer::ComponentIndex;

void DirectX::Renderer::BeginRender()
{
	auto itr = ComponentIndex.begin();
	auto end = ComponentIndex.end();
	while (itr != end) {
		auto renderer = itr->second.lock();
		itr++;
		if (!renderer->m_IsEnable) continue;
		renderer->Render(renderer->transform().lock()->WorldMatrix());
	}
}

DirectX::Renderer::Renderer(EntityID OwnerID,std::string name)
	:
	Component(OwnerID,name)
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

void DirectX::Renderer::OnComponent()
{

}
void DirectX::Renderer::AddComponentIndex(std::weak_ptr<Renderer> instance)
{
	ComponentIndex.emplace(instance.lock()->GetOwnerID(),std::weak_ptr<Renderer>(instance));
}