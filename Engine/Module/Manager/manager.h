#pragma once

class CManager
{
private:
	CManager() = default;
	~CManager() = default;
public:
	static void Initialize();
	static void Uninit();
	static void Update();
	static void FixedUpdate();
	static void DebugRender();
	static void Render(void);
};