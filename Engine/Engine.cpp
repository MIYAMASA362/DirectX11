#include<Windows.h>
#include<string>
#include<map>
#include"Module\DirectX\DirectX.h"


#include"Module\IMGUI\GUI_ImGui.h"
#include"Window\Window.h"

#include"Module\Manager\manager.h"

#include"Engine.h"

#define FRAME_RATE (60)

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
	D3DRenderer::Create();
	if (FAILED(D3DRenderer::GetInstance()->CreateRenderStatus(this->_hWnd, &_RenderStatus, FRAME_RATE)))
	{
		MessageBox(NULL, "RenderStatusの生成に失敗しました。", "EditorWindow", MB_OK);
		return 0;
	}
	
	CManager::Initialize(this->_hWnd,FRAME_RATE);

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
			if (!CManager::IsProcess()) return msg.wParam;

			CManager::SetFrame();

			CManager::Update();
			CManager::FixedUpdate();

			if (!CManager::IsUpdate())
			{
				_RenderStatus->ClearRenderTargetView(Color::gray());

				CManager::Render(_RenderStatus);
				CManager::DebugRender();

				_RenderStatus->End();
			}

		}
	} 
	while (msg.message != WM_QUIT);

	CManager::Finalize();

	delete _RenderStatus;
	D3DRenderer::Destroy();

	return msg.wParam;
}
