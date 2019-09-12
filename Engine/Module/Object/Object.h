#pragma once

namespace DirectX
{
	class Object
	{
	public:
		Object();
		virtual ~Object();
	public:
		virtual void OnDestroy();
		virtual void Destroy();
	};

	//----------------------------------------------------------------
	Object::Object(){
		
	}
	Object::~Object(){
		OnDestroy();
	};
	inline void Object::OnDestroy()
	{
	
	}
	inline void Object::Destroy() 
	{

	}
}