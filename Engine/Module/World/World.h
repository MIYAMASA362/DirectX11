#pragma once

namespace DirectX
{
	//ECS��World�T�O

	class World 
	{
	private:
		const std::string _name;
		bool _active;
	public:
		World(std::string name);
		virtual ~World();
	};
}