#ifndef Bracelet_H
#define Bracelet_H

#include "../../Skill/Skills.h"
#include "../Equipment.h"

namespace FTK::item
{
	class Bracelet : public Equipment
	{
	public: Bracelet() :
		Equipment(
			EmType::Bracelet, EmPart::Accessory,
			"Bracelet", "Bracelet",
			FTK::Attribute{ 0, 1, 0, 0, 0, 0, 0, 0 },
			std::vector<std::shared_ptr<ISkill>>{
				std::make_shared<Heal>(nullptr),
			}
		)
	{

	}
	};
}
#endif // !Bracelet_H