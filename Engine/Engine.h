#pragma once

class RenderStatus;

class EngineWindow final :public System::Window
{
private:
	RenderStatus* _RenderStatus;

public:
	//�R���X�g���N�^
	EngineWindow();
	//�f�X�g���N�^
	~EngineWindow();

	virtual WPARAM MessageLoop() override;
};