#pragma once

#include<CommCtrl.h>

namespace System
{
	class Window;
}

namespace Editor
{
	//*********************************************************************************************************************
	//
	//	FileTreeView
	//		�t�@�C���̃c���[�r���[
	//
	//*********************************************************************************************************************
	class FileTreeView : public System::Window
	{
	private:
		std::string _TreeDirectory;	//�c���[�r���[�ł̃f�B���N�g��

		HWND _TreeView;		//�c���[�r���[
		BOOL _IsDragging;	//�c���[���ړ���
		HIMAGELIST _hImage;	//�C���[�W���X�g

		HWND _ListView;		//���X�g�r���[
		LVCOLUMN _Column;	//���X�g�r���[�\���`��

	public:
		//�R���X�g���N�^
		FileTreeView();
		//�f�X�g���N�^
		virtual ~FileTreeView();

		//�E�B���h�E����
		virtual HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style) override;

		//���[�J���ȃE�B���h�E�v���V�[�W��
		virtual LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);




		//*********************************************************************************************************************
		//
		//	TreeView
		//
		//*********************************************************************************************************************

		//�t�@�C���m�[�h�\��
		void FileViewNode(LPSTR filePath, TVINSERTSTRUCT tv);

		//�c���[���h���b�O�ړ��J�n
		void TreeView_OnBeginDrag(HWND hwndTV, LPNMTREEVIEW lpnmtv);
		//�c���[���h���b�O�ړ���
		void TreeView_OnDragging(HWND hwndParent, HWND hwndTV, LONG xCur, LONG yCur);
		//�c���[�̃h���b�O�ړ��I��
		void TreeView_OnEndDrag(HWND hwndTV);

		//�I�𒆂̃c���[�m�[�h��k���āA�I�����ꂽ�t�@�C���̃p�X�𓾂�
		std::string TreeView_GetFilePath(HTREEITEM item);




		//*********************************************************************************************************************
		//
		//	ListView
		//
		//*********************************************************************************************************************

		//�f�B���N�g�����̃t�@�C�������X�g�r���[�ŕ\��
		void ListView_FileView(std::string DirectoryPath);
		//�I���t�@�C�����J��
		void ListView_FileOpen(LPNMITEMACTIVATE lpnmitem);

		//���X�g�r���[����t�@�C���p�X�𓾂�
		std::string ListView_GetFilePath(HTREEITEM item);
	};
}