#include"main.h"
#include"GUI_ImGui.h"
#include"Window.h"
#include"DirectXStruct.h"
#include"DirectX.h"

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

	//ApplicationÇÃê∂ê¨
	D3DApp::Create(MainWindow);

	return D3DApp::Run(60);
}
