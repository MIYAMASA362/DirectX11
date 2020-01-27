#pragma once

class CCamera;

class CCube :public CGameObject 
{
private:
	ID3D11Buffer* _VertexBuffer;
	CShader* _Shader;

	CTexture* _Texture;
	CCamera* _Camera;
public:
	CCube();
	void Init();
	void Update();
	void Draw();
	void UnInit();

	void SetCamera(CCamera* camera) { _Camera = camera; }
};