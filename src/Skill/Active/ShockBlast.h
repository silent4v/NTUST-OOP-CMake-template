#ifndef ShockBlast_H
#define ShockBlast_H

#include "../ISkill.h"

namespace FTK
{
	class ShockBlast : public ActiveAttack
	{
	public: ShockBlast(std::shared_ptr<Creature> master, double damageRate = .5, EmDamageType damageType = EmDamageType::Magical) :
		ActiveAttack(master, EmSkill::ShockBlast, 1, 3, damageType, damageRate,
			ActiveSkill::TargetSelectCreature(EmTargetSelectCreatureFlag::Enemy), EmSkillRange::All
		)
	{

	}
	public: virtual double RollProbability() const override;
	public: virtual std::string Description() const override
	{
		return "ShockBlast";
	}
	};
}
#endif // !ShockBlast_H