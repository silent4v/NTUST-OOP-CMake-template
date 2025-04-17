#ifndef RitualSword_H
#define RitualSword_H

#include "../Weapon.h"
#include "../../Skill/Skills.h"

namespace FTK::item
{
	class RitualSword : public Weapon
	{
	public: RitualSword() :
		Weapon(
			EmType::RitualSword, "RitualSword", "RitualSword",
			1,
			FTK::Attribute{ 0, 0, 0, 0, 15, 0, 0, 0 }, EmDamageType::Magical,
			std::vector<std::shared_ptr<ISkill>>{
				std::make_shared<ShockBlast>(nullptr),
			}
		  )
	{

	}
	};
}
#endif // !RitualSword_H