#pragma once

class CManager
{

public:
	static void Initialize();
	static void Uninit();
	static void Update();
	static void FixedUpdate();
	static void Render(void);
};