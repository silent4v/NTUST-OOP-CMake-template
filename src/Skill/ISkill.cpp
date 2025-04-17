#include "ISkill.h"

#include "../Entity/Creature.h"
#include "../Battle/IBattleInfo.h"

namespace FTK
{
	Damage ActiveAttack::Damage(int roll) const
	{
		auto master = this->master.lock();
		if (master == nullptr)
			return FTK::Damage{ this->DamageType(), 0 };

		double maxDamage = master->CurrentAttribute().PhysicalAttack;
		if (this->DamageType() == EmDamageType::Magical)
			maxDamage = master->CurrentAttribute().MagicalAttack;
		maxDamage *= this->damageRate;

		int damage = static_cast<double>(roll) / static_cast<double>(this->RollAmount()) * maxDamage;
		return FTK::Damage{ this->DamageType(), damage };
	}

	std::shared_ptr<IBattleInfo> ActiveAttack::BattleInfo(std::vector<std::shared_ptr<Creature>> targets, std::shared_ptr<BattleEntities> battleEntities, int focus)
	{
		return std::make_shared<AttackBattleInfo>(
			this->master,
			targets,
			battleEntities,
			std::static_pointer_cast<ActiveSkill>(this->shared_from_this()),
			focus);
	}

	void ActiveAttack::Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo)
	{
		if (gameInvokeEvent != EmGameInvokeEvent::Battle_UsingSkill)
			return;

		auto attackInfo = std::static_pointer_cast<AttackBattleInfo>(battleInfo);
		auto attacker = attackInfo->Master.lock();
		for (std::size_t targetIndex = 0; targetIndex < attackInfo->Targets.size(); targetIndex++)
		{
			auto target = attackInfo->Targets[targetIndex];
			const auto& targetDefendDamage = attackInfo->TargetDefendDamages[targetIndex];
			if (targetDefendDamage.Fail)
			{
				continue;
			}

			for (const auto& skill : target->Skills())
			{
				skill->Invoke(EmGameInvokeEvent::Battle_WhenDefend, battleInfo);
			}

			int damage = targetDefendDamage.Damage();
			attackInfo->HurtDamages.push_back(damage);
			attackInfo->TotalDamage += damage;

			target->Hurt(damage);
			// TODO: refactor trigger event in Global manager
			for (const auto& skill : target->Skills())
			{
				skill->Invoke(EmGameInvokeEvent::Battle_BeHurt, battleInfo);
			}

			for (const auto& skill : attacker->Skills())
			{
				skill->Invoke(EmGameInvokeEvent::Battle_CauseHarm, battleInfo);
			}
			auto equipments = attacker->Equipments();
			for (const auto& equipment : equipments)
			{
				equipment->Invoke(EmGameInvokeEvent::Battle_CauseHarm, battleInfo);
			}

		}
	}
	EmTargetSelectCreatureFlag operator|(EmTargetSelectCreatureFlag a, EmTargetSelectCreatureFlag b)
	{
		return static_cast<EmTargetSelectCreatureFlag>(static_cast<int>(a) | static_cast<int>(b));
	}
	int ActiveSkill::Roll(int focus) const
	{
		double successfullProbability = this->RollProbability();
		successfullProbability -= this->master.lock()->ReduceDamageProbability();
		if (successfullProbability < 0)
		{
			successfullProbability = 0;
		}

		std::vector<double> probabilities{
			1. - successfullProbability,
			successfullProbability,
		};

		int successfulCount = focus;
		if (focus > rollAmount)
		{
			successfulCount = rollAmount;
			return successfulCount;
		}

		for (int i = 0; i < rollAmount - focus; i++)
		{
			int isSuccessful = Util::RandomSample(probabilities, rand(), 2);
			if (isSuccessful == 1)
			{
				successfulCount++;
			}
		}

		return successfulCount;
	}
}