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
		
		unsigned int id;
		std::string name;
	public:
		Object() :name(typeid(*this).name()),id(0){};
		Object(std::string name) :name(name),id(0){};
		virtual ~Object() 
		{ 
			OnDestroy(); 
		};
	public:
		virtual void OnDestroy() {};
		std::string ObjectName(){
			return name;
		}
	};
}