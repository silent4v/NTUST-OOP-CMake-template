#include "Hammer_Splash.h"

#include "../../Battle/IBattleInfo.h"
#include "../../Buff/Buff.h"

namespace FTK
{
	void Hammer_Splash::Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo) 
	{
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
		if (attackInfo->ActiveSkill->RollAmount() != attackInfo->Roll())
			return;

		auto target = battleInfo->Targets[0];
		target->AddBuff(std::make_shared<buff::Dizziness>(target));

		// splash damage
		int causeDamage = attackInfo->TotalDamage*.5;

		std::vector<std::shared_ptr<Creature>> otherTargets;
		for (auto& otherTarget : battleInfo->BattleEntities->Enemys)
		{
			if (otherTarget == target)
				continue;

			otherTargets.push_back(otherTarget);
		}

		battleInfo->AdditionalBattleInfo.push_back(std::make_shared<AdditionalDamageBattleInfo>(
			this->master, otherTargets, 
			battleInfo->BattleEntities,
			std::static_pointer_cast<ISkill>(this->shared_from_this()), 
			Damage{
				EmDamageType::Physical,
				causeDamage,
			}
		));
		battleInfo->InvokeSkills.push_back(this->ToISkill());
	}
}
