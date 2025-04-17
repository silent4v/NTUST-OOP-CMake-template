#ifndef Run_H
#define Run_H

#include "../ISkill.h"

namespace FTK
{
	class Run : public PassiveSkill
	{
	public: Run (std::shared_ptr<Creature> master) : 
		PassiveSkill(master, EmSkill::Run, 0)
	{
	}

	public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo) override;

	public: virtual std::string Description() const override
	{
		return "Run";
	}


	};
}

#endif // !Run_H
