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
	//�R���X�g���N�^
	IComponentSystem();
	//�f�X�g���N�^
	virtual ~IComponentSystem();

	//
	virtual void SendComponentMessage(unsigned int message) = 0;
};

