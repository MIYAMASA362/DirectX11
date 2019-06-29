#include"main.h"
#include"Window.h"
#include"DirectXStruct.h"
#include"DirectX.h"

using namespace System;

Window* MainWindow;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MainWindow = new Window();
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
