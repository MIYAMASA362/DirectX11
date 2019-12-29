#include<Windows.h>
#include<string>
#include"Module\DirectX\DirectX.h"


#include"Module\IMGUI\GUI_ImGui.h"
#include"Window\Window.h"

#include"Module\Manager\manager.h"

#include"Engine.h"

EngineWindow::EngineWindow()
{
}

EngineWindow::~EngineWindow()
{
}

WPARAM EngineWindow::MessageLoop()
{
	MSG msg;

	//Applicationの生成
	D3DApp::Create(
		this->Get_Window(),
		this->Get_hInstance(),
		60
	);

	CManager::Initialize();

	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// メッセージの翻訳とディスパッチ
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			CManager::Run();
		}
	} 
	while (msg.message != WM_QUIT);

	CManager::Finalize();

	D3DApp::Destroy();

	return msg.wParam;
}
