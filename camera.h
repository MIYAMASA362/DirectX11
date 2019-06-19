#pragma once


class CCamera:public Behaviour
{
private:
	RECT m_Viewport;
public:
	CCamera(){};
public:
	void Initialize()override;
	void Update()override;
	void Render()override;
	void Finalize()override;
};