#include<memory>
#include<typeinfo>
#include<string>
#include"../Object/Object.h"
#include"Component.h"

#include"Module\IMGUI\GUI_ImGui.h"

using namespace DirectX;

void Component::SetEnable(bool isEnable)
{
	this->IsEnable = isEnable;
}

void DirectX::Component::Run(std::weak_ptr<GameObject> gameObject, std::weak_ptr<Transform> transform,Component::Message message)
{
	if (!this->IsEnable) return;
	this->gameObject = gameObject;
	this->transform = transform;
	this->SendBehaviourMessage(message);
}

void DirectX::Component::DebugImGui()
{
	if (ImGui::TreeNode(this->name.c_str())) {
		ImGui::TreePop();
	}
	
}
