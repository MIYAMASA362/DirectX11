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
	inline Object::Object(){
		
	}
	inline Object::~Object(){
		OnDestroy();
	};
	inline void Object::OnDestroy()
	{
	
	}
	inline void Object::Destroy() 
	{

	}
}