#ifndef LeatherArmor_H
#define LeatherArmor_H

#include "../../Skill/Skills.h"
#include "../Equipment.h"

namespace FTK::item
{
	class LeatherArmor : public Equipment
	{
	public: LeatherArmor() :
		Equipment(
			EmType::LeatherArmor, EmPart::Armor,
			"LeatherArmor", "LeatherArmor",
			FTK::Attribute{ 0, 0, 0, 5, 0, 0, 0, 0 },
			std::vector<std::shared_ptr<ISkill>>{
				std::make_shared<Fortify>(nullptr),
			}
		)
	{

	}
	};
}
#endif // !LeatherArmor_H