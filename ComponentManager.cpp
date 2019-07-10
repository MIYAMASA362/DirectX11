#include "ComponentManager.h"

using namespace DirectX;
std::shared_ptr<ComponentManager> ComponentManager::pInstance;

void DirectX::ComponentManager::Create()
{
	if (pInstance.get() == nullptr)
		pInstance = std::shared_ptr<ComponentManager>(new ComponentManager());
}

void DirectX::ComponentManager::Destroy()
{
	if (pInstance.get() != nullptr)
		pInstance.reset();
}

