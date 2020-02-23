#pragma once

#include"Editor\Module\EditorModule.h"

class RenderStatus;
class GameObject;

class Transform;

namespace System
{
	class Window;
}

namespace Editor
{
	//*********************************************************************************************************************
	//
	//	InspectorView
	//
	//*********************************************************************************************************************
	class InspectorView final : public System::Window
	{
	private:
		std::weak_ptr<GameObject> _GameObject;

		EditorWindow* _EditorWindow = nullptr;

		RenderStatus* _RenderStatus = nullptr;

		HWND _ModelView;
		
		//GameObjectの名前変更
		HWND _NameEdit;

		HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style) override;

		bool _IsDelete = false;

	public:
		InspectorView();
		~InspectorView();

		//ウィンドウプロシージャ
		LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
		//ウィンドウ生成
		HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style, EditorWindow* editor, std::shared_ptr<GameObject> target);

		RenderStatus** GetLPRenderStatus() { return &_RenderStatus; };

		bool GetIsDelete() { return _IsDelete; }

	private:

		void Draw(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


		//数値入力カテゴリ
		struct CategoriesEdit
		{
			HWND hEditX, hEditY, hEditZ;
			int x, y, width, height;

			void CreateEdit(HWND Parent, LPARAM lParam, int x, int y, int width, int height, Vector3& value);
			void GetValue(Vector3& value);
			void ResetValue(Vector3& value);
			void DrawValue(HDC hdc, LPSTR Categories);
		};

		class TransformView final : public System::Window
		{
			Transform* _transform;

			CategoriesEdit PositionEdit;
			CategoriesEdit RotationEdit;
			CategoriesEdit ScaleEdit;

			virtual HRESULT Create(HWND hParent, HINSTANCE hInstance, LPSTR lpClassName, LPSTR lpCaption, int x, int y, long width, long height, DWORD style) override { return E_FAIL;};

		public:
			TransformView();
			~TransformView();

			void CreateView(HWND hParent,LPSTR lpClassName,LPSTR lpCaption, int x, int y, long width, long height, DWORD style,Transform* transform);

			virtual LRESULT localWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
		};

		TransformView* _TransformView = nullptr;
	};

}