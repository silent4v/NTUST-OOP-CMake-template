#include "DirtyStuff.h"

#include "../../Battle/IBattleInfo.h"
#include "../../Entity/Creature.h"

namespace FTK::item
{
	void DirtyStuff::Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo)
	{
		Weapon::Invoke(gameInvokeEvent, battleInfo);

		if (gameInvokeEvent != EmGameInvokeEvent::Battle_CauseHarm)
			return;
		if (battleInfo->UseSkill->Type() != ISkill::EmType::ActiveAttack)
			return;
		if (battleInfo->Targets.size() != 1)
			return;

		auto attackInfo = std::static_pointer_cast<AttackBattleInfo>(battleInfo);
		if (attackInfo->TargetDefendDamages[0].Fail)
			return;

		if (attackInfo->ActiveSkill->Range != EmSkillRange::Single)
			return;


		auto target = battleInfo->Targets[0];
		target->AddBuff(std::make_shared<buff::Poisoned>(target, 3));

	}
}