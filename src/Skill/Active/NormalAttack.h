#ifndef NormalAttack_H
#define NormalAttack_H

#include "../ISkill.h"

namespace FTK
{
	class NormalAttack : public ActiveAttack
	{
	public: NormalAttack(std::shared_ptr<Creature> master, int rollAmount, double damageRate = 1., EmDamageType damageType = EmDamageType::Physical) :
		ActiveAttack(master, EmSkill::NormalAttack, 0, rollAmount, damageType, damageRate,
			ActiveAttack::TargetSelectCreature(EmTargetSelectCreatureFlag::Enemy), EmSkillRange::Single
		)
	{

	}
	public: virtual double RollProbability() const override;
	public: virtual std::string Description() const override
	{
		return "Normal Attack";
	}
	};
}
#endif // !NormalAttack_H