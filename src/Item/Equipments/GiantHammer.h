#ifndef GiantHammer_H
#define GiantHammer_H

#include "../../Skill/Skills.h"
#include "../Weapon.h"

namespace FTK::item
{
	class GiantHammer : public Weapon
	{
	public: GiantHammer () :
		Weapon(
			EmType::GiantHammer, "GiantHammer", "GiantHammer",
			5,
			FTK::Attribute{ 0, 0, 20, 0, 0, 0, -15, 0 }, EmDamageType::Physical,
			std::vector<std::shared_ptr<ISkill>>{
				std::make_shared<Hammer_Splash>(nullptr),
			})
	{

	}
	};
}
#endif // !GiantHammer_H