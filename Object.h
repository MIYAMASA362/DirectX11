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
		const unsigned int id;
		const std::string name;
	public:
		Object(std::string name) :name(name),id(0){};
		virtual ~Object() 
		{ 
			OnDestroy(); 
		};
	public:
		virtual void OnDestroy() {};
	};
}