#ifndef WoodenSword_H
#define WoodenSword_H

#include "../Weapon.h"
#include "../../Skill/Skills.h"

namespace FTK::item
{
	class WoodenSword : public Weapon
	{
	public: WoodenSword() :
		Weapon(
			EmType::WoodenSword, "Wooden Sword", "A sword made of wood",
			2,
			FTK::Attribute{ 0, 0, 10, 0, 0, 0, 0, 0 }, EmDamageType::Physical,
			std::vector<std::shared_ptr<ISkill>>{
				std::make_shared<SpeedUp>(nullptr)
			})
	{

	}
	};
}
#endif // !WoodenSword_H