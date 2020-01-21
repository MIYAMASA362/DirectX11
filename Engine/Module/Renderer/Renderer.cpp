#include<algorithm>
#define INCLUDE_CEREAL
#include"Common.h"

#include"Module\DirectX\DirectX.h"

//ECS
#include"Module\ECSEngine.h"

#include"Module\Mesh\Mesh.h"
#include"Renderer.h"

#include"Module\Transform\Transform.h"
#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"

#include"Module\Camera\camera.h"

#include"Module\Material\Material.h"
#include"Module\Shader\Shader.h"

using namespace DirectX;

//2D描画用
//D3DApp::Renderer::SetProjectionMatrix2D();

//*********************************************************************************************************************
//
//	Renderer
//
//*********************************************************************************************************************

//描画
std::vector<Renderer*> Renderer::RendererIndex;

//Renderer
//	コンストラクタ
//
Renderer::Renderer(EntityID OwnerID)
:
	Component(OwnerID)
{
	RendererIndex.push_back(this);
}

//~Renderer
//	デストラクタ
//
Renderer::~Renderer()
{
	auto rEnd = std::remove_if(RendererIndex.begin(), RendererIndex.end(), [this](const Renderer* renderer) {
		return this == renderer;
	});
	RendererIndex.erase(rEnd,RendererIndex.end());
}

//BeginRenderer
//	描画開始
//
void Renderer::BeginRender()
{
	for (auto renderer : RendererIndex)
	{
		if (!renderer->gameObject()->GetActive()) continue;
		if (!renderer->GetEnable()) continue;
		//if (!Camera::GetActiveCamera()->GetVisibility(renderer.lock()->transform()->position())) continue;

		renderer->Render(renderer->gameObject()->transform().lock()->WorldMatrix());
	}
}