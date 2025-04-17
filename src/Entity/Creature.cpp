#include "Creature.h"

#include <algorithm>

#include "../Skill/Active/NormalAttack.h"
#include "../Skill/Active/Escape.h"

namespace FTK
{
	void Creature::reAssignSkillForEquipment()
	{
		this->skills.clear();
		this->skills.push_back(std::make_shared<FTK::Escape>(std::static_pointer_cast<Creature>(this->shared_from_this())));

		auto addSkill = [&](std::shared_ptr<ISkill> skill)
			{
				for (auto selfSkill : this->skills)
				{
					if (selfSkill->SkillType == skill->SkillType)
						return;
				}

				skill->SetMaster(std::static_pointer_cast<Creature>(this->shared_from_this()));
				this->skills.push_back(skill);
			};

		if (this->weapon != nullptr)
		{
			for (auto skill : this->weapon->Skills)
			{
				addSkill(skill);
			}
		}
		else
		{
			this->skills.push_back(std::make_shared<NormalAttack>(std::static_pointer_cast<Creature>(this->shared_from_this()), 1));
		}
		if (this->armor != nullptr)
		{
			for (auto skill : this->armor->Skills)
			{
				addSkill(skill);
			}
		}
		if (this->accessory != nullptr)
		{
			for (auto skill : this->accessory->Skills)
			{
				addSkill(skill);
			}
		}

		std::sort(this->skills.begin(), this->skills.end(), [](const std::shared_ptr<ISkill>& a, const std::shared_ptr<ISkill>& b)
			{
				return a->SkillType < b->SkillType;
			}
		);
	}

	void Creature::TakeOff(std::shared_ptr<Equipment> equipment)
	{
		bool hasEquipment = false;
		switch (equipment->PartType)
		{
		case Equipment::EmPart::Weapon:
			hasEquipment = this->weapon != nullptr;
			break;
		case Equipment::EmPart::Armor:
			hasEquipment = this->armor != nullptr;
			break;
		case Equipment::EmPart::Accessory:
			hasEquipment = this->accessory != nullptr;
			break;
		}
		if (!hasEquipment)
			return;

		for (auto skill : equipment->Skills)
		{
			skill->SetMaster(nullptr);
		}

		equipment->SetMaster(nullptr);
		switch (equipment->PartType)
		{
		case Equipment::EmPart::Weapon:
			this->weapon = nullptr;
			break;
		case Equipment::EmPart::Armor:
			this->armor = nullptr;
			break;
		case Equipment::EmPart::Accessory:
			this->accessory = nullptr;
			break;
		}

		this->reAssignSkillForEquipment();
		this->ReComputeAttribute();
	}
	void Creature::Wear(std::shared_ptr<Equipment> equipment)
	{
		this->TakeOff(equipment);

		equipment->SetMaster(
			std::static_pointer_cast<Creature>(this->shared_from_this())
		);
		switch (equipment->PartType)
		{
		case Equipment::EmPart::Weapon:
			this->weapon = equipment;
			break;
		case Equipment::EmPart::Armor:
			this->armor = equipment;
			break;
		case Equipment::EmPart::Accessory:
			this->accessory = equipment;
			break;
		}

		this->reAssignSkillForEquipment();
		this->ReComputeAttribute();
	}
}