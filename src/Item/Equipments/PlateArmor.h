#ifndef PlateArmor_H
#define PlateArmor_H

#include "../../Skill/Skills.h"
#include "../Equipment.h"

namespace FTK::item
{
	class PlateArmor : public Equipment
	{
	public: PlateArmor() :
		Equipment(
			EmType::PlateArmor, EmPart::Armor,
			"PlateArmor", "PlateArmor",
			FTK::Attribute{ 0, 0, 0, 20, 0, 0, 0, -10 },
			std::vector<std::shared_ptr<ISkill>>{
				std::make_shared<Fortify>(nullptr),
			}
		)
	{

	}
	};
}
#endif // !PlateArmor_H