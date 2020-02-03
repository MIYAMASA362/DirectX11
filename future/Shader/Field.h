#pragma once

#include"shader.h"
#include"ShaderNormal.h"

class CField : public CGameObject
{
private:
	ID3D11Buffer* _VertexBuffer;
	CShaderNormal* _Shader;

	CTexture* _Texture;
	
	CCamera* _Camera;
public:
	CField();
	void Init();
	void Update();
	void Draw();
	void UnInit();

	void SetCamera(CCamera* camera) { _Camera = camera; };
};