#include "Run.h"

#include "../../Entity/Creature.h"

namespace FTK
{
	void Run::Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo)
	{
		if (gameInvokeEvent != EmGameInvokeEvent::Always)
			return;

		auto master = this->master.lock();
		if (!master)
			return;

		master->SetEnsureSuccessfulActionPoint(master->EnsureSuccessfulActionPoint() + 1);
	}
}