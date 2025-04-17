#ifndef Weapon_H
#define Weapon_H

#include "Equipment.h"
#include "../Skill/Active/NormalAttack.h"

namespace FTK::item
{
	template <typename T>
	std::vector<T> vectorAppend(std::vector<T> a, T b)
	{
		a.push_back(b);
		return a;
	}
	class Weapon : public Equipment
	{
	protected: EmDamageType damageType;
	protected: Weapon(EmType type, std::string name, std::string description,
		int rollAmount,
		FTK::Attribute attribute, EmDamageType damageType, std::vector<std::shared_ptr<ISkill>> skills = {}) :
		Equipment(
			type, EmPart::Weapon, name, description,
			attribute,
			vectorAppend<std::shared_ptr<ISkill>>(skills, std::make_shared<NormalAttack>(nullptr, rollAmount, 1., damageType))),
		damageType(damageType)
	{

	}
	};
}
#endif // !Weapon_H