#pragma once


class CScene;


class CManager
{
private:
	static CScene*	m_Scene;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static CScene* GetScene() { return m_Scene; }

};