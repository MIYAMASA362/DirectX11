#pragma once

class EngineWindow final :public System::Window
{
private:

public:
	//コンストラクタ
	EngineWindow();
	//デストラクタ
	~EngineWindow();

	virtual WPARAM MessageLoop() override;
};