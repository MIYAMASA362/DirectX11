#pragma once
#define GUI_ImGui_H

#include<d3d11.h>

#include"Base\imgui.h"
#include"Base\imgui_impl_dx11.h"
#include"Base\imgui_impl_win32.h"

namespace GUI
{
	class guiImGui
	{
	private:
		static guiImGui* pInstance;
	public:
		static void Create(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
		static void Destroy();
	public:
		static void SetFrame();
		static void Render();
	};
}
