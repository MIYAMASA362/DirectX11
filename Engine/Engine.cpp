#include<Windows.h>
#include<string>
#include"Module\DirectX\DirectX.h"


#include"Module\IMGUI\GUI_ImGui.h"
#include"Window\Window.h"

#include"Module\Manager\manager.h"

#include"Engine.h"

//*********************************************************************************************************************
//
//	EngineWindow
//
//*********************************************************************************************************************

//EngineWindow
//	コンストラクタ
//
EngineWindow::EngineWindow()
{
}

//~EngineWindow
//	デストラクタ
//
EngineWindow::~EngineWindow()
{
}

//MessageLoop
//	メッセージ
//
WPARAM EngineWindow::MessageLoop()
{
	MSG msg;

	//Applicationの生成
	D3DApp* d3dapp = new D3DApp();
	d3dapp->Create(this->_hWnd,60);

	D3DApp::Renderer::RegisterDevice(d3dapp);

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

	D3DApp::Renderer::Release();
	delete d3dapp;

	return msg.wParam;
}
