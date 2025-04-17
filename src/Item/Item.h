#ifndef Item_H
#define Item_H

#include <string>
#include <vector>

#include "../Effect/IEffect.h"

namespace FTK
{
	class Creature;
	class Item : public IEffect
	{
	public: enum class EmType
	{
		// Misc
		Godsbeard,		// replenish some health
		GoldenRoot,		// replenish some focus
		TeleportScroll, // teleport to a location
		Tent,			// rest and replenish health and focus

		// Weapon
		WoodenSword,
		Hammer,
		GiantHammer,
		MagicWand,
		RitualSword,
		DirtyStuff,

		// Armor
		PlateArmor,
		LeatherArmor,
		LaurelWreath,
		Robe,
		WoodenShield,

		// Accessory
		Shoes,
		HolyGrail,
		Bracelet,
	};
	public: const EmType Type;
	public: const std::string Name;
	public: const std::string Description;
	public: const bool CanUseWhenBattle = false;

	protected: Item(EmType type, std::string name, std::string description, bool canUseWhenBattle = false)
		: Type(type), Name(name), Description(description), CanUseWhenBattle(canUseWhenBattle)
	{
	}
	public: virtual ~Item() {}

	public: static inline bool IsEquipment(EmType type)
	{
		return type >= EmType::WoodenSword;
	}
	public: static inline bool IsItem(EmType type)
	{
		return type <= EmType::Tent;
	}
	public: virtual bool Use(std::shared_ptr<Creature> master) { return false; };

	public: bool operator<(const Item& other) const
	{
		return this->Type < other.Type;
	}
	};
}
#endif // !Item_H