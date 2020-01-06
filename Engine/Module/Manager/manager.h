#pragma once

class D3DRenderer;
class TimeManager;

//GameManager
//
//
class CManager
{
private:
	static bool _IsUpdate;
	static bool _IsFixedUpdate;

	CManager() = default;
	~CManager() = default;
public:
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
