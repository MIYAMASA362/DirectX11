#pragma once

class Bullet:public MonoBehaviour<Bullet>
{
public:
	const float speed = 0.5f;
public:
	Bullet(EntityID OwnerID):MonoBehaviour(OwnerID,"Bullet")
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
	Shot(EntityID OwnerID) :MonoBehaviour(OwnerID,"Shot") {};
public:
	void Start()override
	{
		
	}

	void Update()override
	{

	}
};