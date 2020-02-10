#pragma once

class CLight : public CGameObject
{
private:
	ID3D11Buffer* m_LightBuffer;
	LIGHT m_Light;

	XMMATRIX m_ViewMatrix;
public:
	CLight();

	void Init();
	void Update();
	void DrawShadow();
	void Draw();
	void UnInit();

	void UpdateSubResource();

	XMMATRIX GetViewMatrix() { return m_ViewMatrix; }
	ID3D11Buffer** GetBuffer() { return &m_LightBuffer; };
};