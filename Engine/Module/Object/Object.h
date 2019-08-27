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
	public:
		Object(){};
		virtual ~Object() 
		{ 
			OnDestroy(); 
		};
	public:
		virtual void OnDestroy() {};
		std::string ObjectTypeName() {
			return typeid(*this).name();
		};
	};
}