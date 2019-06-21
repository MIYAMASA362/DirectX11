#pragma once

namespace DirectX
{
	/**
		@virtual
		+ Destroy
		+ OnDestroy
	*/
	class Object
	{
	protected:
		const unsigned short id;
		const std::string name;
	public:
		Object() :name(typeid(*this).name()),id(0){};
		virtual ~Object() 
		{ 
			OnDestroy(); 
		};
	public:
		virtual void Destroy(){};
		virtual void OnDestroy() {};
	};
}