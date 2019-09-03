#pragma once

class SceneChange:public Behaviour
{
public:
	std::string nextScene;
public:
	SceneChange() :Behaviour("SceneChange") {};
	void Update()
	{
		auto button = this->gameObject.lock()->GetComponent<Button>();
		if (!button.expired() && button.lock()->IsClick())
			SceneManager::LoadScene(nextScene);

		if (Input::GetKeyTrigger(VK_TAB))
			SceneManager::LoadScene(nextScene);
	}
};

class PressMessageScript:public Behaviour
{
public:
	std::string nextScene;
public:
	PressMessageScript() :Behaviour("PressMessageScript") {};
	void Update()
	{
		if (Input::GetKeyTrigger(VK_SPACE))
			SceneManager::LoadScene(nextScene);
	}
};