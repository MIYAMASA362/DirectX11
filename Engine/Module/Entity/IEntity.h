#pragma once

namespace DirectX
{
	using EntityID = unsigned int;

	//Entity Interface
	class IEntity:public Object
	{
	protected:
		const EntityID m_EntityID;
		bool m_IsEnable;
	public:
		IEntity();
		~IEntity() = default;
		const EntityID GetEntityID();
		bool GetIsEnable();
	};
}