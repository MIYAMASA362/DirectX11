#include<Windows.h>
#include<vector>

#include"../resource.h"

#include"Window\Window.h"
#include "NodeView.h"


Editor::NodeColumn::NodeColumn()
	:
	_IsSelect(false),
	_DefaultBkColor(RGB(200, 100, 100))
{

}

Editor::NodeColumn::~NodeColumn()
{
}

void Editor::NodeColumn::AddConectPos(float x, float y)
{

}

LRESULT Editor::NodeColumn::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT paint;
	RECT rect;
	POINT point;

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		_IsSelect = true;
		GetWindowRect(hWnd, &rect);
		GetCursorPos(&point);
		_offsetX = point.x - rect.left;
		_offsetY = point.y - rect.top;
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_LBUTTONUP:
		_IsSelect = false;
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_MOUSEMOVE:
		if (_IsSelect)
		{
			GetCursorPos(&point);
			GetWindowRect(hWnd, &rect);
			MoveWindow(hWnd, point.x - _offsetX, point.y - _offsetY, rect.right - rect.left, rect.bottom - rect.top, TRUE);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd,&paint);
		if (_IsSelect)
			ExtFloodFill(hdc, 1, 1, RGB(0, 100, 0), FLOODFILLSURFACE);
		else
			ExtFloodFill(hdc, 1, 1, _DefaultBkColor, FLOODFILLSURFACE);
		
		EndPaint(hWnd,&paint);
		return 0;
	default:
		break;
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

HRESULT Editor::NodeColumn::Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style)
{
	WNDCLASSEX WndClass = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		CreateSolidBrush(_DefaultBkColor),
		NULL,
		lpClassName,
		NULL
	};
	RegisterClassEx(&WndClass);

	this->_hWnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		lpClassName,
		lpCaption,
		style,
		x,
		y,
		width,
		height,
		hParent,
		NULL,
		hInstance,
		this
	);

	if (!this->_hWnd) {
		MessageBox(NULL, "hWndの設定に失敗しました。", lpCaption, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}



//*********************************************************************************************************************
//
//	NodeView
//
//*********************************************************************************************************************


//NodeView
//
//
Editor::NodeView::NodeView()
{

}

//~NodeView
//
//
Editor::NodeView::~NodeView()
{
	for(auto obj : _NodeColumn)
	{
		delete obj;
	}
}

//Create
//	ウィンドウ生成
//
HRESULT Editor::NodeView::Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style)
{
	WNDCLASS WndClass = {
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		CreateSolidBrush(RGB(200,200,200)),
		MAKEINTRESOURCE(IDR_MENU3),
		lpClassName
	};
	RegisterClass(&WndClass);

	this->_hWnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		lpClassName,
		lpCaption,
		style | WS_CLIPCHILDREN,
		x,
		y,
		width,
		height,
		hParent,
		NULL,
		hInstance,
		this
	);

	if(!this->_hWnd){
		MessageBox(NULL,"hWndの設定に失敗しました。",lpCaption,MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

//localWndProc
//
//
LRESULT Editor::NodeView::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_NODE_ADDNODE:
			AddNodeColumn();
			break;
		default:
			break;
		}
		return 0;
	default:
		break;
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void Editor::NodeView::AddNodeColumn()
{
	RECT rect;
	GetWindowRect(_hWnd,&rect);
	NodeColumn* column = new NodeColumn();

	HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(this->_hWnd, GWLP_HINSTANCE);
	column->Create(
		this->_hWnd,
		hInstance,
		"NodeColumn",
		"NodeColumn",
		rect.left + 100.f,
		rect.top + 100.f,
		100.f,
		80.f,
		WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE
		);
	_NodeColumn.push_back(column);
}

