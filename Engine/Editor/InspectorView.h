#pragma once

#include"Editor\Module\EditorModule.h"

class RenderStatus;
class GameObject;

class Transform;

namespace Editor
{
	//*********************************************************************************************************************
	//
	//	IComponentView
	//		Inspector上に表示されるComponent情報のインターフェイスビュー
	//
	//*********************************************************************************************************************
	class IComponentView : public System::IWindow
	{
	private:
		IComponentView* _Decorator;

	protected:
		unsigned int _width;
		unsigned int _height;

	public:
		IComponentView(unsigned int width,unsigned int height,IComponentView* view = nullptr);
		virtual ~IComponentView();
			
		virtual void Create(unsigned int x,unsigned int y, HWND hParent,HINSTANCE hInstance);

		void Draw();
	};

	//*********************************************************************************************************************
	//
	//	EditView
	//
	//*********************************************************************************************************************
	class EditView : public IComponentView
	{
	private:

	public:
		EditView(unsigned int width,unsigned int height,IComponentView* view = nullptr);
		virtual ~EditView();

		virtual void Create(unsigned int x,unsigned int y,HWND hParent, HINSTANCE hInstance);
	};

	//*********************************************************************************************************************
	//
	//	LabelEditView
	//
	//*********************************************************************************************************************
	class LabelEditView : public EditView
	{
	private:
		LPSTR _label;
	public:
		LabelEditView(unsigned int width,unsigned int height, LPSTR label,IComponentView* view =nullptr);
		virtual ~LabelEditView();

		virtual void Create(unsigned int x,unsigned int y, HWND hParent, HINSTANCE hInstance);
	};

	//*********************************************************************************************************************
	//
	//	TransformView
	//
	//*********************************************************************************************************************
	class TransformView : public IComponentView
	{
	private:
		

	public:
		TransformView(IComponentView* view = nullptr);
		virtual ~TransformView();
	};





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
		
		//Viewコンテンツ
		IComponentView* _View;

		//GameObjectの名前変更
		HWND _NameEdit;

		HWND _AddComponentButton;

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