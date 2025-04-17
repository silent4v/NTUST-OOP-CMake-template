#include "Fortify.h"

#include "../../Entity/Creature.h"
#include "../../Battle/IBattleInfo.h"

namespace FTK
{
	void Fortify::Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo)
	{
		if (gameInvokeEvent != EmGameInvokeEvent::Battle_WhenDefend)
			return;
		if (this->CurrentCoolDowning() > 0)
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

		attackInfo->TargetDefendDamages[0].DamageReductionRate += .1;
		battleInfo->InvokeSkills.push_back(this->ToISkill());
	}
}