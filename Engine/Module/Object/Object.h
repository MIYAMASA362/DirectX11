#pragma once

namespace DirectX
{
	class Object
	{
	protected:
		bool m_IsDestroy = false;
	public:
		Object();
		virtual ~Object();
	public:
		virtual void OnDestroy() {};
		virtual void Destroy();
		bool IsDestroy();
	};

	//----------------------------------------------------------------
	inline Object::Object()
	{
		
	}
	inline Object::~Object()
	{

	}
	inline void Object::Destroy()
	{
		this->m_IsDestroy = true;
	}
	inline bool Object::IsDestroy()
	{
		return this->m_IsDestroy;
	};

}