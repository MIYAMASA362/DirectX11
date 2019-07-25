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
	static void Render(void);
};