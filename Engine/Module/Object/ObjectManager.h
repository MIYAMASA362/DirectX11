#pragma once

namespace DirectX 
{
	class ObjectManager
	{
	private:
		static ObjectIndex m_ObjectIndex;
		static DestroyIndex m_DestroyIndex;
	public:
		static InstanceID AttachID();
		static void AddIndex(std::shared_ptr<Object> instance);
		static void RemoveIndex(InstanceID instanceID);
		static void AddDestroy(InstanceID instanceID);
		static void ClearnUp();
		static std::weak_ptr<Object> GetInstance(InstanceID instanceID);
		static void Release();
	};
}