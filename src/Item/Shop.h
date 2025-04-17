#ifndef Shop_H
#define Shop_H

#include <map>

#include "Item.h"

namespace FTK
{
	class Shop
	{
	public: using ItemAndPrice = std::pair<std::shared_ptr<Item>, int>;
	public: std::map<Item::EmType, ItemAndPrice> ItemMappingPrice;

	public: Shop()
	{

	}

	public: void AddItem(std::shared_ptr<Item> item, int price)
	{
		ItemMappingPrice[item->Type] = std::make_pair(item, price);
	}
	};
}
#endif // !Shop_H