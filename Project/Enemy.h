#pragma once

class Enemy:public Behaviour
{
private:
	const float maxHp = 10.0f;
	float hp = maxHp;
public:
	GameObject* hpBoard;
public:
	Enemy() :Behaviour("Enemy") {};

	void Update()
	{
		
	}

	void Damage()
	{
		this->hp -= 1.0f;
		if (hp <= 0.0f)
		{
			this->gameObject.lock()->Destroy();
			SceneManager::LoadScene("TitleScene");
		}
		else
		{
			Vector3 scale = hpBoard->transform->localScale();
			scale.x = hp / maxHp;
			scale.z = scale.x;
			hpBoard->transform->localScale(scale);
		}
	}

};