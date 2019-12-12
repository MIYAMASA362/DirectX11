#include<Windows.h>
#include<string>
#include"Module\DirectX\DirectX.h"

#include"Module\IMGUI\GUI_ImGui.h"
#include"Window\Window.h"

#include"Module\Manager\manager.h"

using namespace DirectX;
using namespace System;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Window
	Window* MainWindow = new ImGui_Window();
	MainWindow->Create(
		hInstance,
		"MainWindow",
		"DirectX11 ECS",
		1024, 576,
		WS_OVERLAPPEDWINDOW
	);

	//Applicationの生成
	D3DApp::Create(
		MainWindow->Get_Window(),
		MainWindow->Get_hInstance(),
		60
	);

	MSG msg;

	CManager::Initialize();

	while (1)
	{
		//Message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;	// PostQuitMessage()が呼ばれたらループ終了
			}
			else {
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			continue;
		}
		CManager::Run();
	}
	CManager::Finalize();

	D3DApp::Destroy();

	return (int)msg.wParam;
}