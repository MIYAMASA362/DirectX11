#pragma once

namespace DirectX
{
	typedef unsigned int EntityID;
	
	//Entity Interface
	class IEntity:public Object
	{
	protected:
		const EntityID m_id;
	public:
		IEntity(EntityID id);
		~IEntity() = default;
		const EntityID GetEntityID();
	};

	//------------------------------------------------------------------------
	inline IEntity::IEntity(EntityID id)
	:
		m_id(id)
	{

	}
	inline const EntityID IEntity::GetEntityID(){
		return m_id;
	}
}