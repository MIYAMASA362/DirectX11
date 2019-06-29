#pragma once

namespace DirectX
{
	/**
	@virtual
	+ Initialize
	+ Update
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