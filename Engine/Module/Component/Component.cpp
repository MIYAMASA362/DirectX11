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

void DirectX::Component::DebugImGui()
{
	if (ImGui::TreeNode(this->name.c_str())) {
		ImGui::TreePop();
	}
	
}
