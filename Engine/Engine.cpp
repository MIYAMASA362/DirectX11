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
//	�R���X�g���N�^
//
EngineWindow::EngineWindow()
{
}

//~EngineWindow
//	�f�X�g���N�^
//
EngineWindow::~EngineWindow()
{
}

//MessageLoop
//	���b�Z�[�W
//
WPARAM EngineWindow::MessageLoop()
{
	MSG msg;

	//Application�̐���
	D3DRenderer::Create();
	if (FAILED(D3DRenderer::GetInstance()->CreateRenderStatus(this->_hWnd, &_RenderStatus, FRAME_RATE)))
	{
		MessageBox(NULL, "RenderStatus�̐����Ɏ��s���܂����B", "EditorWindow", MB_OK);
		return 0;
	}
	
	CManager::Initialize(this->_hWnd,FRAME_RATE);

	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
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
