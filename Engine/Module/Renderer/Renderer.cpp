#include"Common.h"

#include"Module\DirectX\DirectX.h"

//ECS
#include"Module\ECSEngine.h"

#include"Renderer.h"
#include"Module\Transform\Transform.h"
#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"

#include"Module\Camera\camera.h"

#include"Module\Shader\Shader.h"

using namespace DirectX;

Renderer::RendererIndex Renderer::_RendererIndex;

void DirectX::Renderer::Create()
{
	_RendererIndex.emplace(RendererTarget::RenderTarget2D, RendererList());
	_RendererIndex.emplace(RendererTarget::RenderTarget3D, RendererList());
}

void DirectX::Renderer::Destroy()
{
	_RendererIndex.erase(RendererTarget::RenderTarget3D);
	_RendererIndex.erase(RendererTarget::RenderTarget2D);
}

DirectX::Renderer::Renderer(EntityID OwnerID)
:
	Component(OwnerID)
{
	this->SendComponentMessage = [this](std::string message) 
	{
		if (message == "Start") Start();
	};
}

DirectX::Renderer::~Renderer()
{
	if (_Shader) delete _Shader;
}

void DirectX::Renderer::SetEnable(bool enable)
{
	this->m_IsEnable = enable;
}

bool DirectX::Renderer::GetEnable()
{
	return this->m_IsEnable;
}

void DirectX::Renderer::SetSort(unsigned int sort)
{
	auto* index = &_RendererIndex.at(_RendererTarget);
	auto targert = std::dynamic_pointer_cast<Renderer>(_self.lock());
	index->remove_if([targert](std::weak_ptr<Renderer> renderer) { return renderer.lock() == targert; });
	index->insert(std::next(index->begin(), _sort), targert);
}

void DirectX::Renderer::Start()
{
	auto* index = &_RendererIndex.at(_RendererTarget);
	index->insert(std::next(index->begin(), _sort), std::dynamic_pointer_cast<Renderer>(_self.lock()));
}

void DirectX::Renderer3D::BeginRender()
{
	auto index = _RendererIndex.at(RendererTarget::RenderTarget3D);
	ComponentManager::SendComponentMessage("Render");

	index.remove_if([](std::weak_ptr<Renderer> obj) { return obj.expired(); });

	for(auto renderer:index)
	{
		if (!renderer.lock()->gameObject()->GetActive()) continue;
		if (!renderer.lock()->GetEnable()) continue;
		//if (!Camera::GetActiveCamera()->GetVisibility(renderer.lock()->transform()->position())) continue;
		renderer.lock()->Render(renderer.lock()->gameObject()->transform().lock()->WorldMatrix());
	}
}

DirectX::Renderer3D::Renderer3D(EntityID OwnerID)
:
	Renderer(OwnerID)
{
	_RendererTarget = RendererTarget::RenderTarget3D;
}

DirectX::Renderer3D::~Renderer3D()
{

}

void DirectX::Renderer2D::BeginRender()
{
	auto index = _RendererIndex.at(RendererTarget::RenderTarget2D);

	index.remove_if([](std::weak_ptr<Renderer> obj) { return obj.expired(); });

	for (auto renderer : index)
	{
		if (!renderer.lock()->gameObject()->GetActive()) continue;
		if (!renderer.lock()->GetEnable()) continue;
		D3DApp::Renderer::SetProjectionMatrix2D();
		renderer.lock()->Render(renderer.lock()->transform()->WorldMatrix());
	}
}

DirectX::Renderer2D::Renderer2D(EntityID OwnerID)
:
	Renderer(OwnerID)
{
	_RendererTarget = RendererTarget::RenderTarget2D;
}

DirectX::Renderer2D::~Renderer2D()
{

}