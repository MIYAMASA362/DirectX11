#include<Windows.h>
#include<string>
#include<thread>
#include<vector>
#include<algorithm>
#include<map>
#include<typeinfo>

#include"Module\DirectX\DirectX.h"

#include"../resource.h"

#include"Module\IMGUI\GUI_ImGui.h"
#include"Window\Window.h"

#include"Module\Manager\manager.h"

#include"Editor\MDIWindow.h"
#include"TreeView.h"

#include"InspectorView.h"
#include "Editor.h"

#include"Common.h"
#include"Module\ECSEngine.h"
#include"Module\Module.h"

#define INSPECTOR_TRANSFORM ("TransformView")

//*********************************************************************************************************************
//
//	InspectorView
//
//*********************************************************************************************************************

Editor::InspectorView::InspectorView()
{

}

Editor::InspectorView::~InspectorView()
{
	if (!_TransformView) delete _TransformView;
}

LRESULT Editor::InspectorView::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	RECT rect;
	float width, height;

	if (_GameObject.expired()) DestroyWindow(hWnd);

	switch (uMsg)
	{
		//生成
	case WM_CREATE:

		GetClientRect(hWnd, &rect);
		width = (float)rect.right - rect.left;
		this->_ModelView = CreateWindow(
			EDITOR_INSPECTOR_VIEW,
			"ModelView",
			WS_CHILD | WS_VISIBLE,
			0,
			rect.bottom - (width),
			width,
			width,
			hWnd,
			NULL,
			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
			NULL
		);

		if (FAILED(D3DRenderer::GetInstance()->CreateRenderStatus(this->_ModelView, &_RenderStatus, 60))) {
			MessageBox(NULL, "RenderStatusの生成に失敗しました。", "失敗", MB_OK);
			return 0;
		}

		SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		break;

		//サイズ
	case WM_SIZE:

		GetClientRect(this->_ModelView, &rect);
		height = (float)rect.bottom - rect.top;
		width = (float)rect.right - rect.left;
		MoveWindow(this->_ModelView, 0, HIWORD(lParam) - height, LOWORD(lParam), height, TRUE);

		if (_RenderStatus)
		{
			_RenderStatus->CleanupRenderTargetView();
			_RenderStatus->CleanupDepthStencilView();

			GetClientRect(this->_ModelView, &rect);
			_RenderStatus->GetSwapChain()->ResizeBuffers(0, (UINT)rect.right - rect.left, (UINT)rect.bottom - rect.top, DXGI_FORMAT_UNKNOWN, 0);
			_RenderStatus->SetClientViewport(this->_ModelView);

			_RenderStatus->CreateRenderTargetView();
			_RenderStatus->CreateDepthStencilView();
		}
		break;

		//最小・最大サイズの設定
	case WM_GETMINMAXINFO:
		MINMAXINFO* info;
		info = (MINMAXINFO*)lParam;
		info->ptMinTrackSize.x = 300;
		info->ptMinTrackSize.y = 300;
		break;

		//テキスト描画
		//
		//
	case WM_PAINT:
		this->Draw(hWnd, uMsg, wParam, lParam);
		break;

	case WM_SETFOCUS:
		SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		this->_IsDelete = true;
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}
	return 0;
}

HRESULT Editor::InspectorView::Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style, EditorWindow * editor, std::shared_ptr<GameObject> target)
{
	this->_EditorWindow = editor;
	this->_GameObject = target;
	return Create(hParent, hInstance, lpClassName, lpCaption, x, y, width, height, style);
}

HRESULT Editor::InspectorView::Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style)
{
	WNDCLASS WndClass = {
		CS_CLASSDC,
		DefWindowProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		EDITOR_INSPECTOR_VIEW
	};
	RegisterClass(&WndClass);

	//ウィンドウ生成
	this->_hWnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		lpClassName,
		lpCaption,
		style,
		x,
		y,
		width + (GetSystemMetrics(SM_CXBORDER) + GetSystemMetrics(SM_CXFIXEDFRAME) + GetSystemMetrics(SM_CXFRAME)) * 2,
		height + (GetSystemMetrics(SM_CYBORDER) + GetSystemMetrics(SM_CYFIXEDFRAME) + GetSystemMetrics(SM_CYFRAME)) * 2 + GetSystemMetrics(SM_CYCAPTION),
		hParent,
		NULL,
		hInstance,
		this	//自身のポインタ設定 プロシージャに渡す
	);

	if (!this->_hWnd) {
		MessageBox(NULL, "hWndの設定に失敗しました。", lpCaption, MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

void Editor::InspectorView::Draw(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT paint;
	hdc = BeginPaint(hWnd, &paint);

	std::string text;
	if (_GameObject.expired())
	{
		text = "None";
		TextOut(hdc, 0, 0, text.data(), text.size());
	}
	else
	{
		unsigned int height = 0;
		unsigned int line = 0;

		std::shared_ptr<GameObject> gameObject = _GameObject.lock();
		text = "Name:" + gameObject->GetName();

		TextOut(hdc, line, 0, text.data(), text.size());
		line += 30;

		for (auto component : gameObject->GetComponents())
		{
			if (std::shared_ptr<Transform> result = std::dynamic_pointer_cast<Transform>(component))
			{
				if (!_TransformView)
				{
					_TransformView = new TransformView();

					height = 300;
					_TransformView->CreateView(
						hWnd,
						INSPECTOR_TRANSFORM,
						"Transform",
						0,
						line,
						300,
						height,
						WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
						result.get()
					);
					line += height;
				}
			}
		}
	}
	EndPaint(_hWnd, &paint);
}

Editor::InspectorView::TransformView::TransformView()
{

}

Editor::InspectorView::TransformView::~TransformView()
{

}

void Editor::InspectorView::TransformView::CreateView(HWND hParent, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style, Transform* transform)
{
	this->_transform = transform;

	HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hParent, GWLP_HINSTANCE);

	WNDCLASS wndClass =
	{
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		lpClassName
	};

	RegisterClass(&wndClass);

	_hWnd = CreateWindow(
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
		return;
	}
}

WNDPROC defaultEditWndProc;

//EditWindowProc
//	WindowsAPI Editのプロシージャ
//
LRESULT CALLBACK EditWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

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
	return CallWindowProc(defaultEditWndProc, hWnd, uMsg, wParam, lParam);
}

//CreateEidtWindow
//	WindowsAPI Editの作成 
//
HWND CreateEditWindow(HWND Parent, LPSTR text, int x, int y, int width, int height, LPARAM lParam,unsigned int num)
{
	HWND result = CreateWindow(
		TEXT("EDIT"),
		text,
		WS_CHILD | WS_VISIBLE | ES_LEFT | WS_BORDER,
		x,
		y,
		width,
		height,
		Parent,
		(HMENU)num,
		((LPCREATESTRUCT)(lParam))->hInstance,
		NULL
	);

	//プロシージャを乗っ取る
	defaultEditWndProc = (WNDPROC)SetWindowLongPtr(result, GWLP_WNDPROC, (LONG_PTR)EditWindowProc);

	return result;
}

//GetValueEdit
//	Editから値を取得
//
bool GetValueEdit(HWND hEdit, float& value)
{
	TCHAR text[256];
	GetWindowText(hEdit, text, strlen(text));
	float keep = value;
	try
	{
		value = std::stof(text);
	}
	catch (std::invalid_argument invalid)
	{
		TCHAR text[256];
		sprintf_s(text, "%f", keep);
		SetWindowText(hEdit, text);
		value = keep;
		return true;
	}

	return false;
}

//DrawViewText
//	TextOutをラップした関数
//
void DrawViewText(HDC hdc, int x, int y, LPSTR text)
{
	TextOut(hdc, x, y, text, strlen(text));
}

//localWndProc
//	TransformViewのウィンドウプロシージャ
//
LRESULT Editor::InspectorView::TransformView::localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT paint;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	switch (uMsg)
	{
		//生成
	case WM_CREATE:
		position = _transform->position();
		PositionEdit.CreateEdit(hWnd,lParam,30,20,100,22, position);
		
		rotation = Quaternion::ToEulerAngles(_transform->rotation());
		RotationEdit.CreateEdit(hWnd,lParam,30,110,100,22,rotation);

		scale = _transform->localScale();
		ScaleEdit.CreateEdit(hWnd,lParam,30,200,100,22,scale);

		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &paint);
		DrawViewText(hdc,0,0,"Transform");
		PositionEdit.DrawValue(hdc,"Position");
		RotationEdit.DrawValue(hdc,"Rotation");
		ScaleEdit.DrawValue(hdc,"Scale");
		DefWindowProc(hWnd, uMsg, wParam, lParam);
		EndPaint(hWnd, &paint);
		return 0;
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case EN_KILLFOCUS:
			position = _transform->position();
			PositionEdit.GetValue(position);
			_transform->position(position);

			rotation = _transform->rotation();
			RotationEdit.GetValue(rotation);
			_transform->rotation(Quaternion::Euler(rotation));

			scale = _transform->localScale();
			ScaleEdit.GetValue(scale);
			_transform->localScale(scale);
			return 0;
		case EN_SETFOCUS:
			position = _transform->position();
			PositionEdit.ResetValue(position);

			rotation = Quaternion::ToEulerAngles(_transform->rotation());
			RotationEdit.ResetValue(rotation);

			scale = _transform->scale();
			ScaleEdit.ResetValue(scale);
			return 0;
		}
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//CreateEdit
//	数値入力エディットの生成
//
void Editor::InspectorView::CategoriesEdit::CreateEdit(HWND Parent, LPARAM lParam, int x, int y, int width, int height, Vector3 & value)
{
	TCHAR text[256];
	sprintf_s(text, "%f", value.x);
	hEditX = CreateEditWindow(Parent, text, x, y + height, width, height, lParam, NULL);
	sprintf_s(text, "%f", value.y);
	hEditY = CreateEditWindow(Parent, text, x, y + height * 2, width, height, lParam, NULL);
	sprintf_s(text, "%f", value.z);
	hEditZ = CreateEditWindow(Parent, text, x, y + height * 3, width, height, lParam, NULL);

	this->x = x;
	this->y = y;
	this->height = height;
}

//GetValue
//	エディットから入力数値の取得
//
void Editor::InspectorView::CategoriesEdit::GetValue(Vector3& value)
{
	if (GetValueEdit(hEditX, value.x)) return;
	if (GetValueEdit(hEditY, value.y)) return;
	if (GetValueEdit(hEditZ, value.z)) return;
	return;
}

//ResetValue
//	Valueデータの数値をエディタに表示する
//
void Editor::InspectorView::CategoriesEdit::ResetValue(Vector3& value)
{
	TCHAR text[256];
	sprintf_s(text, "%f", value.x);
	SetWindowText(hEditX, text);
	sprintf_s(text, "%f", value.y);
	SetWindowText(hEditY, text);
	sprintf_s(text, "%f", value.z);
	SetWindowText(hEditZ, text);
}

//DrawValue
//	パラメータ名の表示
//
void Editor::InspectorView::CategoriesEdit::DrawValue(HDC hdc, LPSTR Categories)
{
	DrawViewText(hdc, x, y, Categories);
	DrawViewText(hdc, x, y + height, "X");
	DrawViewText(hdc, x, y + height * 2, "Y");
	DrawViewText(hdc, x, y + height * 3, "Z");
}
