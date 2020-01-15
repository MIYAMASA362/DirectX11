#pragma once

class RenderStatus;

namespace System
{
	class Window;
}

namespace Editor
{
	class EditorWindow;
	class MDIWindow;
	class CommonControlWindow;
	class FileTreeView;

	//*********************************************************************************************************************
	//
	//	InspectorView
	//
	//*********************************************************************************************************************
	class InspectorView final : public System::Window
	{
	private:
		EditorWindow* _EditorWindow = nullptr;

		RenderStatus* _RenderStatus = nullptr;

		HWND _ModelView;

		HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style) override;
	
		bool _IsDelete = false;
	

	public:
		~InspectorView();

		//�E�B���h�E�v���V�[�W��
		LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
		//�E�B���h�E����
		HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style,EditorWindow* editor);

		RenderStatus** GetLPRenderStatus() { return &_RenderStatus; };

		bool GetIsDelete() { return _IsDelete; }


	};

	//*********************************************************************************************************************
	//
	//	SceneWindow
	//
	//*********************************************************************************************************************
	class SceneWindow final : public System::Window
	{
	private:
		RenderStatus* _RenderStatus = nullptr;

	public:
		//�E�B���h�E�v���V�[�W��
		LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
		//�E�B���h�E����
		HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style) override;

		RenderStatus** GetLPRenderStatus() { return &_RenderStatus; };


	};


	//*********************************************************************************************************************
	//
	//	EditorWindow
	//	�@�G�f�B�^�p�̃E�B���h�E ���C���E�B���h�E
	//
	//	TODO : SubWindow �p�~ 
	//
	//	WARNING : �X���b�h�𑀍삵�Ă���̂ŁA���\�[�X�̓ǂݍ��݂Ȃǂɉe����^����\��
	//
	//*********************************************************************************************************************
	class EditorWindow final : public System::Window
	{
	private:
		//�t�@�C���r���[
		FileTreeView* _FileTree = nullptr;

		//�V�[���E�B���h�E
		SceneWindow* _SceneWindow = nullptr;

		std::vector<InspectorView*> _Inspectors;

		HWND _hToolBer;		//�c�[���o�[
		HWND _hColumnSpace; //�G�f�B�^�̈�

		bool _EnableGUI = true;


		void CheckInspectors();

	public:
		//�R���X�g���N�^
		EditorWindow();
		//�f�X�g���N�^
		~EditorWindow();

		//�E�B���h�E����
		HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style) override;

		//�E�B���h�E�v���V�[�W��
		LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		//���b�Z�[�W���[�v
		WPARAM MessageLoop() override;

		void Update();
	};
}