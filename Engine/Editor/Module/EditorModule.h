#pragma once

namespace System
{
	class Window;
}

namespace Editor
{

	//*********************************************************************************************************************
	//
	//	Edit
	//		Editをラップしたクラス
	//
	//*********************************************************************************************************************
	class Edit
	{
	protected:
		HWND _hWnd;
		int _x;
		int _y;

	public:
		Edit();
		virtual ~Edit();

		static LRESULT CALLBACK EditWindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

		void Create(HWND Parent,LPSTR text,DWORD style,int x,int y,int width,int height,HINSTANCE hInstance,unsigned int EditNum);

		void Draw(HDC hdc,LPSTR label);
	};

	class ValueEdit : public Edit
	{
	public:
		//float型数値取得 true = 取得失敗
		bool GetValue(float& value);
	};


	//*********************************************************************************************************************
	//
	//	Button
	//
	//*********************************************************************************************************************
	class Button
	{
	private:
		HWND _hWnd;
	public:
		Button();
		~Button();
	};
}