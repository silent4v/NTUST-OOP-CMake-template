#ifndef DirtyStuff_H
#define DirtyStuff_H

#include "../../Skill/Skills.h"
#include "../Weapon.h"

namespace FTK::item
{
	class DirtyStuff : public Weapon
	{
	public: DirtyStuff() :
		Weapon(
			EmType::DirtyStuff, "DirtyStuff", "Can destroy enemy equipment and add Poisoned",
			3,
			FTK::Attribute{ 0, 0, 0, 0, 0, 0, 0, 0 }, EmDamageType::Physical,
			std::vector<std::shared_ptr<ISkill>>{
				std::make_shared<Destory>(nullptr)
			})
	{

	}

	public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData) override;
	};
}
#endif // !DirtyStuff_H