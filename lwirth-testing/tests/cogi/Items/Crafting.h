#pragma once

#include "Inventory.h"

namespace craft
{
	//void check(Inventory& inv);

	class CraftingRecipe
	{
	public:
		std::vector<Item> m_inputItems;
		std::vector<Item> m_outputItems;

		
		CraftingRecipe(std::initializer_list<Item> inputItems, std::initializer_list<Item> outputItems);
		~CraftingRecipe();

		
	};

	const unsigned length = 0;
	

	//extern CraftingRecipe recipes[length];

}


	