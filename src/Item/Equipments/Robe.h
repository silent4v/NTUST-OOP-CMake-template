#ifndef Robe_H
#define Robe_H

#include "../../Skill/Skills.h"
#include "../Equipment.h"

namespace FTK::item
{
	class Robe : public Equipment
	{
	public: Robe() :
		Equipment(
			EmType::Robe, EmPart::Armor,
			"Robe", "Robe",
			FTK::Attribute{ 0, 0, 0, 0, 0, 10, 0, 0 },
			std::vector<std::shared_ptr<ISkill>>{}
		)
	{

	}
	};
}
#endif // !Robe_H