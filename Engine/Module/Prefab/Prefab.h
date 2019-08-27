#pragma once

namespace DirectX
{
	class Prefab
	{
	public:
		Prefab();
		virtual ~Prefab();
	protected:
		virtual void SetPrefab() = 0;
	};
}