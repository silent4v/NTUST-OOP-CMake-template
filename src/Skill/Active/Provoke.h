#ifndef Provoke_H
#define Provoke_H

#include "../ISkill.h"

namespace FTK
{
	class Provoke : public ActiveSkill
	{
	public: Provoke(std::shared_ptr<Creature> master) :
		ActiveSkill(master, EmSkill::Provoke, 2, 1,
			ActiveSkill::TargetSelectCreature(EmTargetSelectCreatureFlag::Enemy), EmSkillRange::Single
		)
	{

	}

	public: virtual std::shared_ptr<IBattleInfo> BattleInfo(std::vector<std::shared_ptr<Creature>> targets, std::shared_ptr<BattleEntities> battleEntities, int focus) override;
	public: virtual double RollProbability() const override;
	public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo) override;
	public: virtual std::string Description() const override
	{
		return "Provoke";
	}
	};
}
#endif // !Provoke_H