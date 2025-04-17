#ifndef Heal_H
#define Heal_H

#include "../ISkill.h"

namespace FTK
{
	class Heal : public ActiveSkill
	{
	private: double healRate = 1.5;
	public: Heal(std::shared_ptr<Creature> master) :
		ActiveSkill(master, EmSkill::Heal, 0, 2,
			ActiveSkill::TargetSelectCreature(EmTargetSelectCreatureFlag::Teammate|EmTargetSelectCreatureFlag::Self), EmSkillRange::Single
		)
	{

	}

	public: virtual std::shared_ptr<IBattleInfo> BattleInfo(std::vector<std::shared_ptr<Creature>> targets, std::shared_ptr<BattleEntities> battleEntities, int focus) override;
	public: virtual double RollProbability() const override;
	public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo) override;
	public: virtual std::string Description() const override
	{
		return "Heal";
	}
	};
}
#endif // !Heal_H