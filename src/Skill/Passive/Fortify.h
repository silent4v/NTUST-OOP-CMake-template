#ifndef Fortify_H
#define Fortify_H

#include "../ISkill.h"

namespace FTK
{
	class Fortify : public PassiveSkill
	{
	public: Fortify(std::shared_ptr<Creature> master) : PassiveSkill(master, EmSkill::Fortify, 5)
	{
	}

	public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo) override;

	public: virtual std::string Description() const override
	{
		return "Fortify";
	}


	};
}

#endif // !Fortify_H
