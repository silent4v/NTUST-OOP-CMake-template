#ifndef HolyGrail_H
#define HolyGrail_H

#include "../../Skill/Skills.h"
#include "../Equipment.h"

namespace FTK::item
{
	class HolyGrail : public Equipment
	{
	public: HolyGrail() :
		Equipment(
			EmType::Robe, EmPart::Accessory,
			"HolyGrail", "HolyGrail",
			FTK::Attribute{ 0, 0, 0, 0, 0, 30, 0, 0 },
			std::vector<std::shared_ptr<ISkill>>{
				std::make_shared<Heal>(nullptr),
			}
		)
	{

	}
	};
}
#endif // !HolyGrail_H