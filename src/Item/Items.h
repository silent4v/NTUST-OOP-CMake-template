#ifndef Items_H
#define Items_H

#include "Equipments/Equipments.h"
#include "Item.h"

namespace FTK
{
	namespace item
	{
		class Godsbeard : public Item
		{
		public: Godsbeard() :
			Item(EmType::Godsbeard, "Godsbeard", "Replenish some health", true)
		{

		}

		public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData) override
		{

		}
		public: virtual bool Use(std::shared_ptr<Creature> master);
		};

		class GoldenRoot : public Item
		{
		public: GoldenRoot() :
			Item(EmType::GoldenRoot, "GoldenRoot", "Replenish some focus", true)
		{

		}

		public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData) override
		{

		}
		public: virtual bool Use(std::shared_ptr<Creature> master);
		};

		class TeleportScroll : public Item
		{
		public: TeleportScroll() :
			Item(EmType::TeleportScroll, "TeleportScroll", "Teleport to a location")
		{

		}

		public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData) override
		{

		}
		public: virtual bool Use(std::shared_ptr<Creature> master);
		};

		class Tent : public Item
		{
		public: Tent() :
			Item(EmType::Tent, "Tent", "Rest and replenish health and focus")
		{

		}

		public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData) override
		{

		}
		public: virtual bool Use(std::shared_ptr<Creature> master);
		};
	}

	static inline std::shared_ptr<Item> NewItem(Item::EmType type)
	{
		using namespace item;
		if (Item::IsEquipment(type))
			return NewEquipment(type);

		switch (type)
		{
		case Item::EmType::Godsbeard:
			return std::make_shared<Godsbeard>();
		case Item::EmType::GoldenRoot:
			return std::make_shared<GoldenRoot>();
		case Item::EmType::TeleportScroll:
			return std::make_shared<TeleportScroll>();
		case Item::EmType::Tent:
			return std::make_shared<Tent>();
		default:
			return nullptr;
		}
	}
}
#endif // !Items_H