#include"main.h"
#include"DirectXStruct.h"
#include"DirectX.h"
#include"Object.h"
#include"Component.h"
#include"Transform.h"
#include"Tag.h"
#include"GameObject.h"
#include"Renderer.h"

DirectX::MeshRender::MeshRender()
{
	Object::name = "MeshRender";
}

DirectX::MeshRender::~MeshRender()
{
	mesh.reset();
}

void DirectX::MeshRender::OnComponent()
{
	
}

void DirectX::MeshRender::SendBehaviourMessage(Message message)
{
	if (!this->IsEnable)return;
	if (message != Component::Render) return;
	if (!gameObject.lock()->GetActive()) return;
	mesh->Render(this->transform.lock()->WorldMatrix());
}