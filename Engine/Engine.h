#pragma once

class EngineWindow final :public System::Window
{
private:

public:
	//�R���X�g���N�^
	EngineWindow();
	//�f�X�g���N�^
	~EngineWindow();

	virtual WPARAM MessageLoop() override;
};