#pragma once

namespace DirectX
{
	/**
	@virtual
	+ Initialize
	+ Update
	+ Render
	+ Finalize
	+ Destroy
	+ OnDestroy
	*/
	class Behaviour :public Component
	{
	public:
		virtual ~Behaviour() {};
	};
}