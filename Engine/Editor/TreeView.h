#pragma once

namespace System
{
	class Window;
}

namespace Editor
{
	//FileTreeView
	//	�t�@�C���̃c���[�r���[
	//
	class FileTreeView : public System::Window
	{
	private:
		HWND _TreeView;

	public:
		//�R���X�g���N�^
		FileTreeView();
		//�f�X�g���N�^
		virtual ~FileTreeView();

		//�E�B���h�E����
		virtual HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style) override;

		//���[�J���ȃE�B���h�E�v���V�[�W��
		virtual LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		//�t�@�C���m�[�h�\��
		void FileViewNode(LPSTR filePath, TVINSERTSTRUCT tv);
	};
}