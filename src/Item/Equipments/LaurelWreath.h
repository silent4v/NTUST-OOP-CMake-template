#ifndef LaurelWreath_H
#define LaurelWreath_H

#include "../../Skill/Skills.h"
#include "../Equipment.h"

namespace FTK::item
{
	class LaurelWreath : public Equipment
	{
	public: LaurelWreath() :
		Equipment(
			EmType::LaurelWreath, EmPart::Armor,
			"LaurelWreath", "LaurelWreath",
			FTK::Attribute{ 0, 0, 0, 0, 0, 0, 0, 0 },
			std::vector<std::shared_ptr<ISkill>>{}
		  )
	{

	}

	public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData) override;
	};
}
#endif // !LaurelWreath_H