#pragma once

class D3DRenderer;
class TimeManager;

namespace Editor
{
	class EditorWindow;
}

//GameManager
//
//
class CManager
{
private:
	static Editor::EditorWindow* _Editor;

	static bool _IsUpdate;
	static bool _IsFixedUpdate;

	CManager() = default;
	~CManager() = default;
public:
	static void Editor(Editor::EditorWindow* editor) { _Editor = editor; };
	static Editor::EditorWindow& Editor() { return (*_Editor); }

	static void Initialize(HWND hWnd,unsigned int fps);
	static void SetFrame();
	static bool IsProcess();
	static void Update();
	static bool IsUpdate() { return _IsUpdate; }
	static void FixedUpdate();
	static void Render(RenderStatus* renderstatus);
	static void EndFrame();
	static void DebugRender();
	static void Finalize();
};
