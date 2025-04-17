#include "Buff.h"

#include "../Battle/IBattleInfo.h"
#include "../Entity/Creature.h"

namespace FTK
{
	namespace buff
	{
		void Angry::Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData)
		{
			if (gameInvokeEvent != EmGameInvokeEvent::Always)
				return;

			auto master = this->master.lock();
			master->SetReduceDamageProbability(
				master->ReduceDamageProbability() + .3
			);
		}
		void Poisoned::Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData)
		{
			if (gameInvokeEvent != EmGameInvokeEvent::Battle_TurnStart)
				return;

			auto master = this->master.lock();
			int damage = master->CurrentAttribute().HP * .1;
			if (damage <= 0)
				damage = 1;
			master->Hurt(damage);
		}
		void SpeedUp::Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData)
		{
			if (gameInvokeEvent != EmGameInvokeEvent::Always)
				return;

			auto master = this->master.lock();
			auto attribute = master->ModifyAttribute();
			attribute.Speed = attribute.Speed * 1.5;
			master->SetModifyAttribute(attribute);
		}
	}
}