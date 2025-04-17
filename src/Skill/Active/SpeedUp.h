#ifndef SpeedUp_H
#define SpeedUp_H

#include "../ISkill.h"

namespace FTK
{
	class SpeedUp : public ActiveSkill
	{
	public: SpeedUp(std::shared_ptr<Creature> master) :
		ActiveSkill(master, EmSkill::SpeedUp, 3, 2,
			ActiveAttack::TargetSelectCreature(EmTargetSelectCreatureFlag::Self | EmTargetSelectCreatureFlag::Teammate), EmSkillRange::Single
		)
	{

	}

	public: virtual std::shared_ptr<IBattleInfo> BattleInfo(std::vector<std::shared_ptr<Creature>> targets, std::shared_ptr<BattleEntities> battleEntities, int focus) override;
	public: virtual double RollProbability() const override;
	public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo) override;
	public: virtual std::string Description() const override
	{
		return "Escape";
	}
	};
}
#endif // !SpeedUp_H