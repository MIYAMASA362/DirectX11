#pragma once

class CompassScript:public MonoBehaviour<CompassScript>
{
public:
	GameObject* dirObject;
public:
	CompassScript(EntityID OwnerID) :MonoBehaviour(OwnerID,"CompassScript")
	{
	};

	void Update()
	{
		/*Vector3 vec1 = Vector3::forward();
		Quaternion q;
		q = dirObject->transform->localRotation().conjugate();
		q.z = q.y;
		q.y = 0.0f;
		this->transform.lock()->localRotation(q);*/
	}
};