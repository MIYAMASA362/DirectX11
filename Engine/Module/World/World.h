#pragma once

namespace DirectX
{
	//ECS‚ÌWorldŠT”O

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