#pragma once

//GameManager
//
//
class CManager
{
private:
	CManager() = default;
	~CManager() = default;
public:
	static void Initialize();
	static void Run();
	static void Update();
	static void FixedUpdate();
	static void Render();
	static void DebugRender();
	static void Finalize();
};
