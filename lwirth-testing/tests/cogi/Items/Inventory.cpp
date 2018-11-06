#include "Inventory.h"

#include "Crafting.h"

Inventory::Inventory()
{
	for (int i = 0; i < item::length; i++)
	{
		m_items[i].m_id = i;
		m_items[i].m_amount = 0;
	}
}


Inventory::~Inventory()
{
}


unsigned Inventory::getAmount(std::string name)
{
	unsigned id = item::getID(name);
	return m_items[id].m_amount;
}

unsigned Inventory::getAmount(unsigned id)
{
	if (id >= item::length)throw std::exception("non existing item ID");
	else return m_items[id].m_amount;
		
}

void Inventory::setAmount(std::string name, unsigned amount)
{
	checking = true;
	unsigned id = item::getID(name);
	m_items[id].m_amount = amount;
}

void Inventory::setAmount(unsigned id, unsigned amount)
{
	checking = true;
	m_items[id].m_amount = amount;
}

void Inventory::addAmount(std::string name, int addition)
{
	checking = true;
	unsigned id = item::getID(name);
	m_items[id].m_amount += addition;

}

void Inventory::addAmount(unsigned id, int addition)
{
	checking = true;
	m_items[id].m_amount += addition;
}

void Inventory::checkCrafting()
{
	if (checking)
	{
		//craft::check(*this);
		checking = false;
	}
}

