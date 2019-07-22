#include<string>
#include<list>
#include<memory>
#include<d3d11.h>
#include<DirectXMath.h>

//DirectX
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

//Component
#include"Module\Object\Object.h"
#include"Module\Component\Component.h"

//Component Module
#include"Renderer.h"

#include"Module\Transform\Transform.h"
#include"Module\Tag\Tag.h"
#include"Module\GameObject\GameObject.h"

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