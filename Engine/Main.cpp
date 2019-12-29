#include<Windows.h>
#include"Window\Window.h"

#include"Editor\Editor.h"
#include"Engine.h"

using namespace System;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Window
	Window* MainWindow = new Editor::EditorWindow();

	MainWindow->Create(
		hInstance,
		"MainWindow",
		"DirectX11 ECS",
		1024, 576,
		WS_OVERLAPPEDWINDOW
	);

	//MainLoop
	return (int)MainWindow->MessageLoop();
}
