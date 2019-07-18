#pragma once

#include<typeinfo>
#include"main.h"
#include"DirectXStruct.h"
#include"Object.h"
#include"Component.h"
#include"Transform.h"
#include"Behaviour.h"
#include"Tag.h"
#include"GameObject.h"

namespace DirectX
{
	class Collision;

	class Collider:public Component
	{
	private:
		bool IsTrigger;			//•¨—‹““®‚·‚é‚©	
	public:
		Vector3 center;			//’†SˆÊ’u
	public:
		Collider() {};
		virtual ~Collider() {};
		void SetTrigger(bool isTrigger)
		{
			this->IsTrigger = isTrigger;
		}
		bool GetTrigger(){
			return IsTrigger;
		}
	public:
		//Collision
		virtual void OnCollisionEnter(std::weak_ptr<Collision> other) {};
		virtual void OnCollisionStay(std::weak_ptr<Collision> other) {};
		virtual void OnCollisionExit(std::weak_ptr<Collision> other) {};

		//Trigger
		virtual void OnTriggerEnter(std::weak_ptr<Collider> other){};
		virtual void OnTriggerStay(std::weak_ptr<Collider>other) {};
		virtual void OnTriggerExit(std::weak_ptr<Collider>other) {};
	public:
		static void OnTrigger(std::weak_ptr<Collider> trigger1,std::weak_ptr<Collider> trigger2);
	};

	//Box
	class BoxCollier final:public Collider
	{
	public:
		Vector3 scale;			//‘å‚«‚³
	public:
		BoxCollier() = default;
		virtual ~BoxCollier() {};
	};

	//Sphere
	class SphereCollider final:public Collider
	{
	public:
		float raidus;			//”¼Œa
	public:
		SphereCollider() = default;
		virtual ~SphereCollider() {};
	};

	//Collision
	class Collision
	{
	private:
		Collider* collider;
	};
}