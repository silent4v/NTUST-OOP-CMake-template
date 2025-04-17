#ifndef Equipments_H
#define Equipments_H

#include "Bracelet.h"
#include "DirtyStuff.h"
#include "GiantHammer.h"
#include "Hammer.h"
#include "HolyGrail.h"
#include "LaurelWreath.h"
#include "LeatherArmor.h"
#include "MagicWand.h"
#include "PlateArmor.h"
#include "RitualSword.h"
#include "Robe.h"
#include "Shoes.h"
#include "WoodenShield.h"
#include "WoodenSword.h"

namespace FTK
{
	static inline std::shared_ptr<Equipment> NewEquipment(Item::EmType type)
	{
		using namespace item;
		switch (type)
		{
		case Item::EmType::WoodenSword:
			return std::make_shared<WoodenSword>();
		case Item::EmType::Hammer:
			return std::make_shared<Hammer>();
		case Item::EmType::GiantHammer:
			return std::make_shared<GiantHammer>();
		case Item::EmType::MagicWand:
			return std::make_shared<MagicWand>();
		case Item::EmType::RitualSword:
			return std::make_shared<RitualSword>();
		case Item::EmType::DirtyStuff:
			return std::make_shared<DirtyStuff>();

		case Item::EmType::PlateArmor:
			return std::make_shared<PlateArmor>();
		case Item::EmType::LeatherArmor:
			return std::make_shared<LeatherArmor>();
		case Item::EmType::LaurelWreath:
			return std::make_shared<LaurelWreath>();
		case Item::EmType::Robe:
			return std::make_shared<Robe>();
		case Item::EmType::WoodenShield:
			return std::make_shared<WoodenShield>();

		case Item::EmType::Shoes:
			return std::make_shared<Shoes>();
		case Item::EmType::HolyGrail:
			return std::make_shared<HolyGrail>();
		case Item::EmType::Bracelet:
			return std::make_shared<Bracelet>();
		default:
			return nullptr;
		}
	}
}

#endif // !Equipments_H