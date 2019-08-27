#include<Windows.h>
#include<d3d11.h>

#include"GUI_ImGui.h"
using namespace GUI;

guiImGui* guiImGui::pInstance = nullptr;

void GUI::guiImGui::Create(HWND hwnd,ID3D11Device* device,ID3D11DeviceContext* context)
{
	if (pInstance != nullptr) return;
	pInstance = new guiImGui();

	//ImGui‚Ì‰Šú‰»ˆ—
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device,context);

	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;
	colors[ImGuiCol_WindowBg] = ImVec4(0.0f,0.0f,0.0f,0.8f);
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


