#pragma once

struct CAMERA
{
	XMFLOAT4 Position;
};

class CCamera:public CGameObject
{
private:
	RECT m_Viewport;
	XMMATRIX	 m_ViewMatrix;
	XMMATRIX	 m_ProjectionMatrix;

	ID3D11Buffer *_CameraBuffer;
	CAMERA _Constant;
public:
	CCamera();

	void Init();
	void Update();
	void Draw();
	void UnInit();

	XMMATRIX GetViewMatrix() { return m_ViewMatrix; };
	XMMATRIX GetProjectionMatrix() { return m_ProjectionMatrix; };
};