#ifndef WoodenShield_H
#define WoodenShield_H

#include "../../Skill/Skills.h"
#include "../Equipment.h"

namespace FTK::item
{
	class WoodenShield : public Equipment
	{
	public: WoodenShield() :
		Equipment(
			EmType::WoodenShield, EmPart::Armor,
			"WoodenShield", "WoodenShield",
			FTK::Attribute{ 0, 0, 0, 10, 0, 0, 0, 0 },
			std::vector<std::shared_ptr<ISkill>>{
				std::make_shared<Provoke>(nullptr),
			}
		)
	{

	}
	};
}
#endif // !WoodenShield_H