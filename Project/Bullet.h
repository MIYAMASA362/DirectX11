#pragma once

class Bullet:public Behaviour
{
public:
	const float speed = 0.5f;
public:
	Bullet():Behaviour("Bullet")
	{
		
	};
public:
	void Update()
	{
		this->transform.lock()->position(this->transform.lock()->position()+this->transform.lock()->forward() * speed);
	}
};

class Shot:public Behaviour
{
public:
	Shot() :Behaviour("Shot") {};
public:
	void Initialize()override
	{
		
	}

	void Update()override
	{
		if (Input::GetKeyTrigger('Q'))
		{
			auto bullet = GameObject::Instantiate(this->gameObject.lock().get());
			bullet->AddComponent<MeshRender>()->SetMesh<Model>()->GetAsset("Miku");
			bullet->AddComponent<Bullet>();
		}
	}
};