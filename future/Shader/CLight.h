#pragma once

class CLight :public CGameObject
{
private:
	ID3D11Buffer* m_Buffer;
	LIGHT m_Context;
public:
	CLight();
	virtual ~CLight();

	LIGHT& GetLight() { return m_Context; };
	ID3D11Buffer** GetBuffer() { return &m_Buffer; }

	void Draw() override;
};