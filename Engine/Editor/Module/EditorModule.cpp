#include<Windows.h>
#include<string>

#include "EditorModule.h"

//*********************************************************************************************************************
//
//	Edit
//
//*********************************************************************************************************************

Editor::Edit::Edit()
{

}

Editor::Edit::~Edit()
{
}

WNDPROC DefaultWindowProc;

LRESULT Editor::Edit::EditWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
			//フォーカス切り
			//
			//
		case VK_RETURN:
			SetFocus(GetParent(hWnd));
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return CallWindowProc(DefaultWindowProc, hWnd, uMsg, wParam, lParam);
}

void Editor::Edit::Create(HWND Parent, LPSTR text, DWORD style, int x, int y, int width, int height, HINSTANCE hInstance, unsigned int EditNum)
{
	_x = x;
	_y = y;

	_hWnd = CreateWindow(
		"EDIT",
		text,
		style,
		x,
		y,
		width,
		height,
		Parent,
		(HMENU)EditNum,
		hInstance,
		NULL
	);

	DefaultWindowProc = (WNDPROC)SetWindowLongPtr(this->_hWnd,GWLP_WNDPROC,(LONG_PTR)EditWindowProc);
}

void Editor::Edit::Draw(HDC hdc,LPSTR label)
{
	SIZE size;
	TextOut(hdc,_x,_y,label,strlen(label));
}

bool Editor::ValueEdit::GetValue(float & value)
{
	TCHAR text[256];
	GetWindowText(this->_hWnd,text,strlen(text));
	float keep = value;
	try
	{
		value = std::stof(text);
	}
	catch(std::invalid_argument invalid)
	{
		sprintf_s(text,"%f",keep);
		SetWindowText(this->_hWnd,text);
		value = keep;
		return true;
	}

	return false;
}
