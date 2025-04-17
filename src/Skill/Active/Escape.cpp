#include "Escape.h"

#include "../../Entity/Creature.h"
#include "../../Battle/IBattleInfo.h"

namespace FTK
{
	std::shared_ptr<IBattleInfo> Escape::BattleInfo(std::vector<std::shared_ptr<Creature>> targets, 
		std::shared_ptr<BattleEntities> battleEntities, int focus) 
	{
		return std::make_shared<EffectBattleInfo>(
			this->master,
			targets,
			battleEntities,
			std::dynamic_pointer_cast<ActiveSkill>(this->shared_from_this()),
			focus,
			IBattleInfo::EmType::Espace
		);
	}
	double Escape::RollProbability() const
	{
		auto master = this->master.lock();

		double probability =
			static_cast<double>(master->CurrentAttribute().HP) /
			static_cast<double>(master->ModifyAttribute().HP + master->CurrentAttribute().PhysicalDefense + master->CurrentAttribute().MagicalDefense);
		probability = probability * master->CurrentAttribute().Speed;
		if (probability > 98.)
			probability = 98.;
		return probability / 100.;
	}
	void Escape::Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo)
	{
		return;
	}
}