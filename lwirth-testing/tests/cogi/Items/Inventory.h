#pragma once

#include <vector>
#include "Item.h"


class Inventory
{
public:
	Inventory();
	~Inventory();

private:
	std::vector<Item> m_items = std::vector<Item>(item::length);
	bool checking = false;

public:
	unsigned getAmount(std::string name);
	unsigned getAmount(unsigned id);
	void setAmount(std::string name, unsigned amount);
	void setAmount(unsigned id, unsigned amount);
	void addAmount(std::string name, int addition);
	void addAmount(unsigned id, int addition);

	void checkCrafting();

};

