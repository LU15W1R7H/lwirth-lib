#include "Item.h"

#include <algorithm>




std::vector<ItemType*> item::types = {
	
};

const unsigned item::length = item::types.size();


ItemType::ItemType(unsigned id, std::string name)
	: m_id(id)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	m_name = name;
}

ItemType::~ItemType()
{
}

void ItemType::interact(Cogicorpus& cogi)
{
	//default: don't interact with it
	//if (m_oneTimeUse)cogi.m_inventory.addAmount(this->m_id, -1);
}


Item::Item()
{
}

Item::Item(unsigned id, unsigned amount)
	: m_id(id), m_amount(amount)
{
	
}

Item::Item(std::string name, unsigned amount)
	: m_id(item::getID(name)), m_amount(amount)
{
}

const ItemType& Item::type()
{
	return *item::types[m_id];
}


ItemType* item::get(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	for (int i = 0; i < item::length; i++)
	{
		if (item::types[i]->m_name.compare(name) == 0)return item::types[i];
	}

	throw std::exception("item not found by name");
	return nullptr;
}

unsigned item::getID(std::string name)
{
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	for (int i = 0; i < item::length; i++)
	{
		if (item::types[i]->m_name.compare(name) == 0)return i;
	}

	throw std::exception("item not found by name");
	return -1;
}

