#include "SpeedUp.h"

#include "../../Battle/IBattleInfo.h"
#include "../../Buff/Buff.h"

namespace FTK
{
	std::shared_ptr<IBattleInfo> SpeedUp::BattleInfo(std::vector<std::shared_ptr<Creature>> targets, std::shared_ptr<BattleEntities> battleEntities, int focus)
	{
		return std::make_shared<EffectBattleInfo>(
			this->master,
			targets,
			battleEntities,
			std::static_pointer_cast<ActiveSkill>(this->shared_from_this()),
			focus
		);
	}
	double SpeedUp::RollProbability() const
	{
		auto master = this->master.lock();
		if (master == nullptr)
		{
			return 0.;
		}

		return static_cast<double>(master->CurrentAttribute().HitRate) / 100.;
	}
	void SpeedUp::Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo)
	{
		if (gameInvokeEvent != EmGameInvokeEvent::Battle_UsingSkill)
			return;

		auto effectInfo = std::static_pointer_cast<EffectBattleInfo>(battleInfo);
		if (effectInfo->Fail)
			return;
		if (effectInfo->Roll() != this->RollAmount())
		{
			effectInfo->Fail = true;
			return;
		}

		battleInfo->Targets[0]->AddBuff(
			std::make_shared<buff::SpeedUp>(battleInfo->Targets[0], 1)
		);
	}
}