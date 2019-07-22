#include<Windows.h>
#include<string>
#include<d3d11.h>
#include<DirectXMath.h>
#include"Module\IMGUI\GUI_ImGui.h"
#include"Window\Window.h"
#include"Module\DirectX\DirectXStruct.h"
#include"Module\DirectX\DirectX.h"

using namespace DirectX;
using namespace System;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window* MainWindow = new ImGui_Window();
	MainWindow->Create(
		hInstance,
		"MainWindow",
		"DirectX11 Lefact",
		1024, 576,
		WS_OVERLAPPEDWINDOW
	);

	//Application‚Ì¶¬
	D3DApp::Create(
		MainWindow->Get_Window(),
		MainWindow->Get_hInstance()
	);

	return D3DApp::Run(60);
}
