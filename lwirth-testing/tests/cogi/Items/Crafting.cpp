#include "Crafting.h"


namespace craft
{

	/*CraftingRecipe recipes[length] =
	{

	};*/

	/*void check(Inventory& inv)
	{
		
		for (int i = 0; i < length; i++)
		{
			bool craftable = true;
			CraftingRecipe &rec = recipes[i];
			for (Item in : rec.m_inputItems)
			{
				if (inv.getAmount(in.m_id) < in.m_amount)
				{
					craftable = false;
					break;
				}
			}
			if (craftable)
			{
				for (Item in : rec.m_inputItems)
				{
					inv.addAmount(in.m_id, -1*in.m_amount);
				}

				for (Item out : rec.m_outputItems)
				{
					inv.addAmount(out.m_id, out.m_amount);
				}
			}
		}

		
	}*/


	CraftingRecipe::~CraftingRecipe() {}

	CraftingRecipe::CraftingRecipe(std::initializer_list<Item> inputItems, std::initializer_list<Item> outputItems)
	{
		for (Item input : inputItems) m_inputItems.push_back(input);
		for (Item output : outputItems) m_outputItems.push_back(output);
		
		
	}

}