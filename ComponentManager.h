#pragma once

#include<memory>
#include<list>
#include<map>

namespace DirectX
{
	class GameObject;
	class Component;

	class ComponentManager final
	{
	private:
		static std::shared_ptr<ComponentManager> pInstance;
		std::map<GameObject, std::list<Component>> pIndex;
	public:
		void Create();
		void Destroy();
	};
}