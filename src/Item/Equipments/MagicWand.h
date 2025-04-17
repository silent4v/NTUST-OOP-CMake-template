#ifndef MagicWand_H
#define MagicWand_H

#include "../Weapon.h"
#include "../../Skill/Skills.h"

namespace FTK::item
{
	class MagicWand : public Weapon
	{
	public: MagicWand() :
		Weapon(
			EmType::MagicWand, "MagicWand", "Magic wand",
			3,
			FTK::Attribute{ 0, 0, 0, 0, 10, 0, 0, 0 }, EmDamageType::Magical,
			std::vector<std::shared_ptr<ISkill>>{
				std::make_shared<ShockBlast>(nullptr),
				std::make_shared<Heal>(nullptr)
			}
		  )
	{

	}
	};
}
#endif // !MagicWand_H