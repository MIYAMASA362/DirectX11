#pragma once

class Item
{
public:
	Texture* m_texture;
};

class ItemSlotScript:public MonoBehaviour<ItemSlotScript>
{
public:
	ItemSlotScript(EntityID OwnerID) :MonoBehaviour(OwnerID,"ItemSlotScript"){};
public:
	void Update()
	{
		
	}
};