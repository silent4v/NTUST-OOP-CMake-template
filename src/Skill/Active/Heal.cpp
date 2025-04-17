#include "Heal.h"

#include "../../Entity/Creature.h"
#include "../../Battle/IBattleInfo.h"

namespace FTK
{
	std::shared_ptr<IBattleInfo> Heal::BattleInfo(std::vector<std::shared_ptr<Creature>> targets, std::shared_ptr<BattleEntities> battleEntities, int focus)
	{
		auto master = this->master.lock();
		int maxValue = master->CurrentAttribute().MagicalAttack * this->healRate;

		return std::make_shared<HealBattleInfo>(
			this->master,
			targets,
			battleEntities,
			std::static_pointer_cast<ActiveSkill>(this->shared_from_this()),
			focus
		);
	}
	double Heal::RollProbability() const
	{
		auto master = this->master.lock();
		if (master == nullptr)
		{
			return 0.;
		}

		return static_cast<double>(master->CurrentAttribute().HitRate) / 100.;
	}
	void Heal::Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo)
	{
		if (gameInvokeEvent != EmGameInvokeEvent::Battle_UsingSkill)
			return;

		auto master = this->master.lock();
		auto healInfo = std::static_pointer_cast<HealBattleInfo>(battleInfo);
		if (healInfo->Fail)
			return;

		auto maxHealValue = master->CurrentAttribute().MagicalAttack * this->healRate;
		auto healValue = static_cast<double>(healInfo->Roll()) / static_cast<double>(this->RollAmount()) * maxHealValue;
		for (std::size_t targetIndex = 0; targetIndex < healInfo->Targets.size(); targetIndex++)
		{
			auto target = healInfo->Targets[targetIndex];

			target->Heal(healValue);

			healInfo->HealValue = healValue;
		}
	}
}
