#pragma once

namespace DirectX
{
	class Object
	{
		friend class ObjectManager;
	private:
		const InstanceID m_InstanceID;
	public:
		static void Destroy(Object* obj);

		Object();
		virtual ~Object();
		
		InstanceID GetInstanceID();
		
		virtual void Destroy();
	protected:
		virtual void OnDestroy();
	};

	inline InstanceID DirectX::Object::GetInstanceID()
	{
		return m_InstanceID;
	}
}