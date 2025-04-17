#ifndef Equipment_H
#define Equipment_H

#include <vector>

#include "../Entity/Attribute.h"
#include "../Skill/ISkill.h"
#include "Item.h"

namespace FTK
{
	class Creature;
	class Equipment : public Item
	{
	public: enum class EmPart
	{
		Weapon,
		Armor,
		Accessory,
	};
	public: const EmPart PartType;

	public: const FTK::Attribute Attribute;

	public: const std::vector<std::shared_ptr<ISkill>> Skills;

	protected: std::weak_ptr<Creature> master;

	protected: Equipment(
		EmType type,
		EmPart part, std::string name, std::string description,
		const FTK::Attribute& attribute, const std::vector<std::shared_ptr<ISkill>>& skills = {})
		:
		Item(type, name, description),
		PartType(part), Attribute(attribute), Skills(skills)
	{
	}

	public: void SetMaster(std::shared_ptr<Creature> master)
	{
		this->master = master;
	}

	public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> effectData) override;
	};
}
#endif // !Equipment_H