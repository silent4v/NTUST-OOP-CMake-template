#ifndef PlayerManager_H

#define PlayerManager_H

#include <memory>

#include "../Item/Shop.h"
#include "../Map/Map.hpp"
#include "../Util/Singleton.hpp"

namespace FTK
{
	class PlayerManager : public Util::Singleton<PlayerManager>
	{
	private: friend class Util::Singleton<PlayerManager>;

	private: PlayerManager() = default;

	private: int money = 600;
	public: inline int GetMoney() const { return money; }
	public: inline void SetMoney(int money) { this->money = money; }

	private: std::map<Item::EmType, int> items;
	public: inline const std::map<Item::EmType, int>& GetItems() const { return items; }
	private: std::map<Item::EmType, int> alreadyEquippedItems;
	public: inline const std::map<Item::EmType, int>& AlreadyEquippedItems() const { return alreadyEquippedItems; }

	public: void BuyItem(Item::EmType item, int price)
	{
		if (money < price)
			return;

		money -= price;
		items[item]++;
	}

	public: void EquipItem(Item::EmType item)
	{
		if (items[item] <= 0)
			return;

		items[item]--;
		alreadyEquippedItems[item]++;
	}
	public: void TakeOffItem(Item::EmType item)
	{
		if (alreadyEquippedItems[item] <= 0)
			return;

		alreadyEquippedItems[item]--;
		items[item]++;

		if (alreadyEquippedItems[item] == 0)
		{
			alreadyEquippedItems.erase(item);
		}
	}

	public: void RemoveItem(Item::EmType item)
	{
		if (items[item] <= 0)
			return;

		items[item]--;
		if (items[item] == 0)
		{
			items.erase(item);
		}
	}

		  // TODO: remove this member
	public: std::shared_ptr<FTK::Map> Map;

	public: void Init(std::shared_ptr<FTK::Map> map)
	{
		this->Map = map;
	}
	};
}
#endif // !PlayerManager_H
