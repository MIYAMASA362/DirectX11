#pragma once

class RenderStatus;

class EngineWindow final :public System::Window
{
private:
	RenderStatus* _RenderStatus;

public:
	//コンストラクタ
	EngineWindow();
	//デストラクタ
	~EngineWindow();

	virtual WPARAM MessageLoop() override;
};