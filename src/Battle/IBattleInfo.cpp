#include "IBattleInfo.h"

namespace FTK
{
	ActiveBattleInfo::ActiveBattleInfo(
		EmType type,
		std::weak_ptr<FTK::Creature> master,
		std::vector<std::shared_ptr<FTK::Creature>> targets,
		std::shared_ptr<FTK::BattleEntities> battleEntities,
		std::shared_ptr<FTK::ActiveSkill> useSkill,
		int useFocus)
		:
		IBattleInfo(type, master, targets, battleEntities, useSkill),
		ActiveSkill(std::static_pointer_cast<FTK::ActiveSkill>(useSkill)),
		UseFocus(useFocus)
	{

	}

	AttackBattleInfo::AttackBattleInfo(
		std::weak_ptr<FTK::Creature> master,
		std::vector<std::shared_ptr<FTK::Creature>> targets,
		std::shared_ptr<FTK::BattleEntities> battleEntities,
		std::shared_ptr<FTK::ActiveSkill> useSkill, int useFocus
		, int debugRollAmount)
		:
		ActiveBattleInfo(EmType::AttackInfo, master, targets, battleEntities, useSkill, useFocus),
		ActiveAttack(std::static_pointer_cast<FTK::ActiveAttack>(useSkill))
	{
		if (debugRollAmount == -1)
			this->roll = this->ActiveAttack->Roll(this->UseFocus);
		else
			this->roll = debugRollAmount;

		auto skillDamage = this->ActiveAttack->Damage(this->roll);

		if (this->roll == 0)
		{
			for (const auto& target : this->Targets)
			{
				this->TargetDefendDamages.push_back(
					AttackInfo{ Damage{}, Damage{}, true }
				);
			}
		}
		else
		{
			for (const auto& target : this->Targets)
			{
				this->TargetDefendDamages.push_back(
					AttackInfo{
						skillDamage,
						Damage{
							skillDamage.Type,
							skillDamage.Type == EmDamageType::Physical
								? target->PhysicalDefense()
								: target->CurrentAttribute().MagicalDefense
						},
						false,
					}
				);
			}
		}
	}
	AdditionalDamageBattleInfo::AdditionalDamageBattleInfo(
		std::weak_ptr<FTK::Creature> master,
		std::vector<std::shared_ptr<FTK::Creature>> targets,
		std::shared_ptr<FTK::BattleEntities> battleEntities,
		std::shared_ptr<ISkill> skill,
		FTK::Damage damage)
		:
		IBattleInfo(EmType::AdditionalDamageInfo, master, targets, battleEntities, skill),
		Damage(damage)
	{
		for (const auto& target : this->Targets)
		{
			this->TargetDefendDamages.push_back(
				AttackInfo{
					this->Damage,
					FTK::Damage{
						this->Damage.Type,
						this->Damage.Type == EmDamageType::Physical
							? target->PhysicalDefense()
							: target->CurrentAttribute().MagicalDefense
					},
					false,
				}
			);
		}
	}

	EffectBattleInfo::EffectBattleInfo(
		std::weak_ptr<FTK::Creature> master,
		std::vector<std::shared_ptr<FTK::Creature>> targets,
		std::shared_ptr<FTK::BattleEntities> battleEntities,
		std::shared_ptr<FTK::ActiveSkill> useSkill, int useFocus, EmType type,
		int debugRollAmount)
		:
		ActiveBattleInfo(type, master, targets, battleEntities, useSkill, useFocus)
	{
		if (debugRollAmount == -1)
			this->roll = this->ActiveSkill->Roll(this->UseFocus);
		else
			this->roll = debugRollAmount;

		this->Fail = this->roll == 0;
	}

	HealBattleInfo::HealBattleInfo(
		std::weak_ptr<FTK::Creature> master,
		std::vector<std::shared_ptr<FTK::Creature>> targets,
		std::shared_ptr<FTK::BattleEntities> battleEntities,
		std::shared_ptr<FTK::ActiveSkill> useSkill,
		int useFocus)
		:
		ActiveBattleInfo(EmType::HealInfo, master, targets, battleEntities, useSkill, useFocus)
	{
		this->roll = this->ActiveSkill->Roll(this->UseFocus);
		if (roll == 0)
		{
			this->Fail = true;
			return;
		}
	}
}