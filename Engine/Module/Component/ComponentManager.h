#pragma once

class IEntity;

//*********************************************************************************************************************
//
//	ComponentManager
//
//*********************************************************************************************************************
class ComponentManager final
{
private:
	//インスタンス
	static ComponentManager* pInstance;
	//Component
	std::vector<std::weak_ptr<IComponent>> _ComponentIndex;


private:
	//コンストラクタ
	ComponentManager();
	//デストラクタ
	~ComponentManager();


public:

	//インスタンス生成
	static void Create();
	//インスタンス破棄
	static void Release();
	//インスタンス
	static ComponentManager* GetInstance() { return pInstance; };


	//ComponentにMessage送信
	void SendComponentMessage(std::string message);

	//ComponentIndexへ追加
	void RegisterComponent(std::weak_ptr<IComponent> component);
	//ComponentIndexから削除
	void ReleaseComponent(IComponent* component);
};