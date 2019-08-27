#pragma once

class Item
{
public:
	Texture* m_texture;
};

class ItemSlotScript:public Behaviour
{
public:
	ItemSlotScript() :Behaviour("ItemSlotScript"){};
public:
	void Update()
	{
		
	}
};