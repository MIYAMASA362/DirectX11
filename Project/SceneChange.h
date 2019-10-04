#pragma once

class SceneChange:public MonoBehaviour<SceneChange>
{
public:
	std::string nextScene;
public:
	SceneChange(EntityID OwnerID) :MonoBehaviour(OwnerID,"SceneChange") {};
	void Update()
	{
		/*auto button = this->gameObject.lock()->GetComponent<Button>();
		if (!button.expired() && button.lock()->IsClick())
			SceneManager::LoadScene(nextScene);

		if (Input::GetKeyTrigger(VK_TAB))
			SceneManager::LoadScene(nextScene);*/
	}
};

class PressMessageScript:public MonoBehaviour<PressMessageScript>
{
public:
	std::string nextScene;
public:
	PressMessageScript(EntityID OwnerID) :MonoBehaviour(OwnerID,"PressMessageScript") {};
	void Update()
	{
		/*if (Input::GetKeyTrigger(VK_SPACE))
			SceneManager::LoadScene(nextScene);*/
	}
};