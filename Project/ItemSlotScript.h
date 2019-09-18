#pragma once

class Item
{
public:
	Texture* m_texture;
};

class ItemSlotScript:public MonoBehaviour<ItemSlotScript>
{
public:
	ItemSlotScript() :MonoBehaviour("ItemSlotScript"){};
public:
	void Update()
	{
		
	}
};