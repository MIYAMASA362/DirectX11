#pragma once

class Enemy:public MonoBehaviour<Enemy>
{
private:
	const float maxHp = 10.0f;
	float hp = maxHp;
public:
	GameObject* hpBoard;
public:
	Enemy(EntityID OwnerID) :MonoBehaviour(OwnerID) {};

	void Update()
	{
		
	}

	void Damage()
	{
		/*this->hp -= 1.0f;
		if (hp <= 0.0f)
		{
			this->gameObject.lock()->Destroy();
			SceneManager::LoadScene(SceneManager::GetSceneByName("TitleScene"));
		}
		else
		{
			Vector3 scale = hpBoard->transform->localScale();
			scale.x = hp / maxHp;
			scale.z = scale.x;
			hpBoard->transform->localScale(scale);
		}*/
	}

};