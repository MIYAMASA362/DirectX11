#pragma once

class SceneChange:public MonoBehaviour<SceneChange>
{
public:
	std::string nextScene;
public:
	SceneChange(EntityID OwnerID) :MonoBehaviour(OwnerID) {};
	void Update()
	{
		if (Input::GetKeyTrigger(VK_TAB))
			SceneManager::LoadScene(SceneManager::GetSceneByName(nextScene));
	}
};

class PressMessageScript:public MonoBehaviour<PressMessageScript>
{
public:
	std::string nextScene;
public:
	PressMessageScript(EntityID OwnerID) :MonoBehaviour(OwnerID) {};
	void Update()
	{
		/*if (Input::GetKeyTrigger(VK_SPACE))
			SceneManager::LoadScene(nextScene);*/
	}
};