#ifndef Hammer_Splash_H
#define Hammer_Splash_H

#include "../ISkill.h"

namespace FTK
{
	class Hammer_Splash : public PassiveSkill
	{
	public: Hammer_Splash(std::shared_ptr<Creature> master) : PassiveSkill(master, EmSkill::Hammer_Splash, 0)
	{
	}

	public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo) override;

	public: virtual std::string Description() const override
	{
		return "Hammer Splash";
	}


	};
}

#endif // !Hammer_Splash_H
