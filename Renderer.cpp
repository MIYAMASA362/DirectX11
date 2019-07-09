#include"main.h"
#include"DirectXStruct.h"
#include"DirectX.h"
#include"Object.h"
#include"Component.h"
#include"Transform.h"
#include"Tag.h"
#include"GameObject.h"
#include"Renderer.h"

void DirectX::MeshRender::OnComponent()
{
	
}

void DirectX::MeshRender::SendBehaviourMessage(Message message)
{
	if (message != Component::Render) return;
	mesh->Render(this->transform.lock()->WorldMatrix());
}