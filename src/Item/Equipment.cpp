#include "Equipment.h"

#include "../Entity/Creature.h"

namespace FTK
{
	void Equipment::Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData)
	{
		if (gameInvokeEvent != EmGameInvokeEvent::Always)
			return;

		auto master = this->master.lock();
		auto attribute = master->ModifyAttribute();
		attribute += this->Attribute;

		master->SetModifyAttribute(attribute);
	}
}