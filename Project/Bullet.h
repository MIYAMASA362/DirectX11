#pragma once

class Bullet:public MonoBehaviour<Bullet>
{
public:
	const float speed = 0.5f;
public:
	Bullet():MonoBehaviour("Bullet")
	{
		
	};
public:
	void Update()
	{
		//this->transform.lock()->position(this->transform.lock()->position()+this->transform.lock()->forward() * speed);
	}
};

class Shot:public MonoBehaviour<Shot>
{
public:
	Shot() :MonoBehaviour("Shot") {};
public:
	void Initialize()override
	{
		
	}

	void Update()override
	{

	}
};