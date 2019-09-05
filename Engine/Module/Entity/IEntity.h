#pragma once

namespace DirectX
{
	typedef unsigned int EntityID;
	
	//Entity Interface
	class IEntity
	{
	protected:
		const EntityID id;
	public:
		IEntity(EntityID id);
		const EntityID GetEntityID();
	};

	//------------------------------------------------------------------------
	inline IEntity::IEntity(EntityID id)
	:
		id(id)
	{

	}
	inline const EntityID IEntity::GetEntityID()
	{
		return id;
	}
}