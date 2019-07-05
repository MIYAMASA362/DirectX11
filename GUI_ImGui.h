#pragma once
#define GUI_ImGui_H

#include"imgui\imgui.h"
#include"imgui\imgui_impl_dx11.h"
#include"imgui\imgui_impl_win32.h"

namespace DirectX
{
	class D3DApp;
}

using namespace DirectX;

namespace GUI
{
	class guiImGui
	{
	private:
		static guiImGui* pInstance;
	public:
		static void Create(D3DApp* application);
		static void Destroy();
	public:
		static void SetFrame();
		static void Render();
	};
}
