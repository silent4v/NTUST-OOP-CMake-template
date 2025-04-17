#ifndef Creature_H
#define Creature_H

#include "../Buff/Buff.h"
#include "../Item/Equipment.h"
#include "../Skill/ISkill.h"
#include "Attribute.h"
#include "Entity.h"

namespace FTK
{
	class Creature : public Entity
	{
	protected: Attribute baseAttribute;
	public: inline const Attribute& BaseAttribute() const { return this->baseAttribute; }
	protected: Attribute modifyAttribute;
	public: inline const Attribute& ModifyAttribute() const { return this->modifyAttribute; }
	public: inline void SetModifyAttribute(const Attribute& attribute) { this->modifyAttribute = attribute; }
	protected: Attribute currentAttribute;
	public: inline const Attribute& CurrentAttribute() const { return this->currentAttribute; }

	public: bool IsAlive() const { return this->currentAttribute.HP > 0; }

	protected: std::vector<std::shared_ptr<ISkill>> skills;
	public: inline const std::vector<std::shared_ptr<ISkill>>& Skills() const { return this->skills; }
	public: inline void SetSkills(const std::vector<std::shared_ptr<ISkill>>& skills) { this->skills = skills; }

	protected: std::shared_ptr<Equipment> weapon;
	protected: std::shared_ptr<Equipment> armor;
	protected: std::shared_ptr<Equipment> accessory;

	public: Creature() = default;
	public: Creature(
		const std::string& name, UI::Pixel renderPixel, EmType type, const Vector2D& position,
		const Attribute& attribute)
		: Entity(name, renderPixel, type, position)
	{
		this->baseAttribute = attribute;
		this->currentAttribute = attribute;
		this->modifyAttribute = attribute;

		this->ReComputeAttribute();
	}

	public: void TakeOff(std::shared_ptr<Equipment> equipment);
	public: void Wear(std::shared_ptr<Equipment> equipment);

	public: inline std::shared_ptr<Equipment> Weapon() const { return this->weapon; }
	public: inline std::shared_ptr<Equipment> Armor() const { return this->armor; }
	public: inline std::shared_ptr<Equipment> Accessory() const { return this->accessory; }
	public: inline std::shared_ptr<Equipment> GetEquipment(Equipment::EmPart partType) const
	{
		switch (partType)
		{
		case Equipment::EmPart::Weapon:
			return this->Weapon();
		case Equipment::EmPart::Armor:
			return this->Armor();
		case Equipment::EmPart::Accessory:
			return this->Accessory();
		}
		return nullptr;
	}
	public: inline std::vector<std::shared_ptr<Equipment>> Equipments() const
	{
		std::vector<std::shared_ptr<Equipment>> result;
		if (this->weapon != nullptr)
			result.push_back(this->weapon);
		if (this->armor != nullptr)
			result.push_back(this->armor);
		if (this->accessory != nullptr)
			result.push_back(this->accessory);

		return result;
	}

	private: std::vector<std::shared_ptr<IBuff>> buffs;
	public: inline std::vector<std::shared_ptr<IBuff>> Buffs() const { return this->buffs; }

	public: inline void AddBuff(std::shared_ptr<IBuff> buff)
	{
		for (int i = 0; i < this->buffs.size(); i++)
		{
			if (this->buffs[i]->Type == buff->Type)
			{
				this->buffs[i] = buff;
				this->ReComputeAttribute();
				return;
			}
		}

		this->buffs.push_back(buff);
		this->ReComputeAttribute();
	}
	public: inline void RemoveBuff(std::shared_ptr<IBuff> buff)
	{
		auto it = std::find(this->buffs.begin(), this->buffs.end(), buff);
		if (it != this->buffs.end())
		{
			this->buffs.erase(it);
		}

		this->ReComputeAttribute();
	}
	public: inline void ClearBuff()
	{
		this->buffs.clear();

		this->ReComputeAttribute();
	}

	private: double physicalDefenseRate = .0;
	public: inline double PhysicalDefenseRate() const { return this->physicalDefenseRate; }
	public: inline void SetPhysicalDefenseRate(double value) { this->physicalDefenseRate = value; }
	public: inline int PhysicalDefense() const
	{
		return static_cast<int>(this->currentAttribute.PhysicalDefense * (1. + this->physicalDefenseRate));
	}
	private: double reduceDamageProbability = .0;
	public: inline double ReduceDamageProbability() const { return this->reduceDamageProbability; }
	public: inline void SetReduceDamageProbability(double value) { this->reduceDamageProbability = value; }

	private: int ensureSuccessfulActionPoint = 0;
	public: inline int EnsureSuccessfulActionPoint() const { return this->ensureSuccessfulActionPoint; }
	public: inline void SetEnsureSuccessfulActionPoint(int value) { this->ensureSuccessfulActionPoint = value; }

	private: void reAssignSkillForEquipment();
	public: inline void ReComputeAttribute()
	{
		int hp = this->currentAttribute.HP;
		int focus = this->currentAttribute.Focus;

		this->reduceDamageProbability = .0;
		this->physicalDefenseRate = .0;
		this->ensureSuccessfulActionPoint = 0;

		this->modifyAttribute = this->baseAttribute;

		// TODO: effect sequence manager
		{
			for (auto& buff : this->buffs)
			{
				buff->Invoke(EmGameInvokeEvent::Always, nullptr);
			}
			for (auto& skill : this->skills)
			{
				skill->Invoke(EmGameInvokeEvent::Always, nullptr);
			}
			if (this->weapon != nullptr)
				this->weapon->Invoke(EmGameInvokeEvent::Always, nullptr);
			if (this->armor != nullptr)
				this->armor->Invoke(EmGameInvokeEvent::Always, nullptr);
			if (this->accessory != nullptr)
				this->accessory->Invoke(EmGameInvokeEvent::Always, nullptr);
		}

		if (this->modifyAttribute.HP < 0)
			this->modifyAttribute.HP = 0;
		if (this->modifyAttribute.Focus < 0)
			this->modifyAttribute.Focus = 0;

		if (this->modifyAttribute.PhysicalAttack < 0)
			this->modifyAttribute.PhysicalAttack = 0;
		if (this->modifyAttribute.PhysicalDefense < 0)
			this->modifyAttribute.PhysicalDefense = 0;

		if (this->modifyAttribute.MagicalAttack < 0)
			this->modifyAttribute.MagicalAttack = 0;
		if (this->modifyAttribute.MagicalDefense < 0)
			this->modifyAttribute.MagicalDefense = 0;

		if (this->modifyAttribute.HitRate < 0)
			this->modifyAttribute.HitRate = 0;
		if (this->modifyAttribute.Speed < 0)
			this->modifyAttribute.Speed = 0;

		this->currentAttribute = this->modifyAttribute;
		if (this->currentAttribute.HP > hp)
			this->currentAttribute.HP = hp;
		if (this->currentAttribute.Focus > focus)
			this->currentAttribute.Focus = focus;
	}

	public: void Hurt(int damage)
	{
		this->currentAttribute.HP -= damage;
		if (this->currentAttribute.HP < 0)
		{
			this->currentAttribute.HP = 0;
		}

		this->ReComputeAttribute();
	}
	public: void Heal(int value)
	{
		this->currentAttribute.HP += value;
		if (this->currentAttribute.HP > this->modifyAttribute.HP)
			this->currentAttribute.HP = this->modifyAttribute.HP;
	}
	public: void RestoreFocus(int value)
	{
		this->currentAttribute.Focus += value;
		if (this->currentAttribute.Focus > this->modifyAttribute.Focus)
			this->currentAttribute.Focus = this->modifyAttribute.Focus;
	}
	public: void UseFocus(int value)
	{
		this->currentAttribute.Focus -= value;

		this->ReComputeAttribute();
	}
	};
}
#endif // !Creature_H