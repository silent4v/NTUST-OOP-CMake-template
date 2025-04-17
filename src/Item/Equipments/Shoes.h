#ifndef Shoes_H
#define Shoes_H

#include "../../Skill/Skills.h"
#include "../Equipment.h"

namespace FTK::item
{
	class Shoes : public Equipment
	{
	public: Shoes() :
		Equipment(
			EmType::Shoes, EmPart::Accessory,
			"Shoes", "Shoes",
			FTK::Attribute{ 0, 0, 0, 0, 0, 0, 0, 5 },
			std::vector<std::shared_ptr<ISkill>>{
				std::make_shared<Run>(nullptr)
			}
		  )
	{

	}
	};
}
#endif // !Shoes_H