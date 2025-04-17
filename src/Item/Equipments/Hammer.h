#ifndef Hammer_H
#define Hammer_H

#include "../../Skill/Skills.h"
#include "../Weapon.h"

namespace FTK::item
{
	class Hammer : public Weapon
	{
	public: Hammer() :
		Weapon(
			EmType::Hammer, "Hammer", "Hammer",
			4,
			FTK::Attribute{ 0, 0, 15, 0, 0, 0, -15, 0 }, EmDamageType::Physical,
			std::vector<std::shared_ptr<ISkill>>{
				std::make_shared<Hammer_Splash>(nullptr),
				std::make_shared<Provoke>(nullptr),
			})
	{

	}
	};
}
#endif // !Hammer_H