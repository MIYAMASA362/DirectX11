#pragma once

namespace DirectX
{
	class Object
	{
	protected:
		const unsigned short id;
		const std::string name;
	public:
		Object() :name(typeid(*this).name()),id(0){};
		virtual ~Object() {};
	};
}