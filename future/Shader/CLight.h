#pragma once

class CLight : public CGameObject
{
private:
	D3D11_VIEWPORT m_Viewport;

	XMMATRIX m_ViewMatrix;
	XMMATRIX m_ProjectionMatrix;
public:
	CLight();

	void Init();
	void Update();
	void DrawShadow();
	void Draw();
	void UnInit();

	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
};