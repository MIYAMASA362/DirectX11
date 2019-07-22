#pragma once

class SceneChange:public Behaviour
{
public:
	std::string nextScene;
public:
	void Update()
	{
		if (Input::GetKeyTrigger(VK_SPACE))
			SceneManager::LoadScene(nextScene);
	}
};