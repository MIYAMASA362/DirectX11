#pragma once

//*********************************************************************************************************************
//
//	IComponentSystem
//
//*********************************************************************************************************************
class IComponentSystem
{
private:

public:
	//コンストラクタ
	IComponentSystem();
	//デストラクタ
	virtual ~IComponentSystem();

	//
	virtual void SendComponentMessage(unsigned int message) = 0;
};

