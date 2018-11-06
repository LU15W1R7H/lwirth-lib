#pragma once

#include <string>
#include <vector>
class Cogicorpus;


class ItemType
{

public:
	ItemType(unsigned id, std::string name);
	~ItemType();

	unsigned m_id;
	std::string m_name;

	bool m_oneTimeUse = false;

	void virtual interact(Cogicorpus& cogi);
};

namespace item
{

	extern std::vector<ItemType*> types;
	extern const unsigned length;

	ItemType *get(std::string name);
	unsigned getID(std::string name);

}

class Item
{
public:
	Item();
	Item(unsigned id, unsigned amount = 0);
	Item(std::string name, unsigned amount = 0);
	~Item() {}

	unsigned m_id = 0;
	unsigned m_amount = 0;
	const ItemType& type();
};






