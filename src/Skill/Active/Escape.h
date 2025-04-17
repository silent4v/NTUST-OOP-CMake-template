#ifndef Escape_H
#define Escape_H

#include "../ISkill.h"

namespace FTK
{
	class Escape : public ActiveSkill
	{
	public: Escape(std::shared_ptr<Creature> master) :
		ActiveSkill(master, EmSkill::Escape, 0, 1,
			ActiveAttack::TargetSelectCreature(EmTargetSelectCreatureFlag::Self), EmSkillRange::Single
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
#endif // !Escape_H