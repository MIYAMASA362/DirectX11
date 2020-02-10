#include<Windows.h>
#include<vector>
#include<memory>
#include"Window\Window.h"

#include"Editor\Editor.h"
#include"Engine.h"

using namespace System;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Window
	Window* MainWindow = new Editor::EditorWindow();

	MainWindow->Create(
		NULL,
		hInstance,
		"MainWindow",
		"DirectX11 ECS",
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1024, 
		576,
		WS_OVERLAPPEDWINDOW
	);

	//MainLoop
	auto result = (int)MainWindow->MessageLoop();

	delete MainWindow;

	return result;
}
