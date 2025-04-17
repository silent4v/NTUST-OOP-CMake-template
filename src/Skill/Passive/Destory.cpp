#include "Destory.h"

#include <vector>

#include "../../Manager/PlayerManager.hpp"

#include "../../Battle/IBattleInfo.h"
#include "../../Entity/Creature.h"

namespace FTK
{
	void Destory::Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo)
	{
		if (gameInvokeEvent != EmGameInvokeEvent::Battle_CauseHarm)
			return;

		if (battleInfo->UseSkill->Type() != ISkill::EmType::ActiveAttack)
			return;
		if (battleInfo->Targets.size() != 1)
			return;

		auto attackInfo = std::static_pointer_cast<AttackBattleInfo>(battleInfo);
		if (attackInfo->TargetDefendDamages[0].Fail)
			return;

		if (attackInfo->ActiveSkill->Range != EmSkillRange::Single)
			return;

		auto target = battleInfo->Targets[0];
		std::vector<Equipment::EmPart> parts;
		if (target->Weapon() != nullptr)
		{
			parts.push_back(Equipment::EmPart::Weapon);
		}
		if (target->Armor() != nullptr)
		{
			parts.push_back(Equipment::EmPart::Armor);
		}
		if (target->Accessory())
		{
			parts.push_back(Equipment::EmPart::Accessory);
		}

		if (parts.size() == 0)
			return;

		auto part = parts[rand() % parts.size()];
		auto equipment = target->GetEquipment(part);

		target->TakeOff(equipment);
		PlayerManager::Instance().TakeOffItem(equipment->Type);
		PlayerManager::Instance().RemoveItem(equipment->Type);
	}
}