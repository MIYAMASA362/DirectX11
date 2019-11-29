#pragma once

class Bullet:public MonoBehaviour<Bullet>
{
public:
	const float speed = 0.5f;
public:
	Bullet(EntityID OwnerID):MonoBehaviour(OwnerID)
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
	Shot(EntityID OwnerID) :MonoBehaviour(OwnerID) {};
public:
	void Start()override
	{
		
	}

	void Update()override
	{

	}
};

class RotationBox:public MonoBehaviour<RotationBox>
{
public :
	RotationBox(EntityID OwnerID) :MonoBehaviour(OwnerID) { };
public:
	void Start() override
	{
	
	}

	void Update() override
	{
		Quaternion q = this->transform()->rotation();
		q *= Quaternion::AngleAxisToEuler(0.1f,Vector3::up());
		this->transform()->rotation(q);
	}
};