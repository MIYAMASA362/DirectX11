#include"main.h"
#include "GUI_ImGui.h"
#include"DirectXStruct.h"
#include"DirectX.h"

using namespace GUI;

guiImGui* guiImGui::pInstance = nullptr;

void GUI::guiImGui::Create(D3DApp * application)
{
	if (pInstance != nullptr) return;
	pInstance = new guiImGui();

	//ImGui‚Ì‰Šú‰»ˆ—
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(application->GetWindow());
	ImGui_ImplDX11_Init(application->GetDevice(),application->GetDeviceContext());
}

void GUI::guiImGui::Destroy()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void GUI::guiImGui::SetFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void GUI::guiImGui::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


