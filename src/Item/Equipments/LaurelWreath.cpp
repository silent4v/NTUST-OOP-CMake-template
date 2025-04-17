#include "LaurelWreath.h"

#include "../../Entity/Creature.h"

namespace FTK::item
{
	void LaurelWreath::Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData)
	{
		if (gameInvokeEvent != EmGameInvokeEvent::Always)
		{
			return;
		}
		auto master = this->master.lock();
		if (master == nullptr)
			return;

		master->SetPhysicalDefenseRate(master->PhysicalDefenseRate() + 0.1);
	}
}