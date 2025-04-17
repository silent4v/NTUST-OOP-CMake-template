#ifndef Destory_H
#define Destory_H

#include "../ISkill.h"

namespace FTK
{
	class Destory : public PassiveSkill
	{
	public: Destory(std::shared_ptr<Creature> master) : PassiveSkill(master, EmSkill::Destory, 0)
	{
	}

	public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo) override;

	public: virtual std::string Description() const override
	{
		return "Destory";
	}


	};
}

#endif // !Destory_H
