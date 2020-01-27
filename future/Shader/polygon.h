#pragma once


#include "shader.h"


class CPolygon : public CGameObject
{
private:

	ID3D11Buffer*	m_VertexBuffer = NULL;
	CShader*		m_Shader;

	CTexture*	m_Texture;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();


};
