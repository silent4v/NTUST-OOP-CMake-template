#ifndef ISkill_H
#define ISkill_H

#include <memory>
#include <set>
#include <string>
#include <vector>

#include "../Effect/IEffect.h"
#include "../GameEvent/EmGameInvokeEvent.h"
#include "../Util/BitFlags.hpp"
#include "../Util/RandomSample.hpp"

namespace FTK
{
	enum class EmSkill
	{
		NormalAttack,
		Escape,
		SpeedUp,
		Heal,
		ShockBlast,
		Provoke,

		Hammer_Splash,
		Fortify,
		Destory,
		Run,
	};
	static std::string SkillToString(const EmSkill& skill)
	{
		switch (skill)
		{
		case EmSkill::NormalAttack:
			return "NormalAttack";
		case EmSkill::Escape:
			return "Escape";
		case EmSkill::ShockBlast:
			return "ShockBlast";
		case EmSkill::Heal:
			return "Heal";
		case EmSkill::Provoke:
			return "Provoke";
		case EmSkill::SpeedUp:
			return "SpeedUp";
		case EmSkill::Fortify:
			return "Fortify";
		case EmSkill::Destory:
			return "Destory";
		case EmSkill::Run:
			return "Run";
		default:
			return "UnDefine";
		}
	}

	enum class EmDamageType
	{
		Physical,
		Magical
	};
	static std::string DamageTypeToString(const EmDamageType& type)
	{
		switch (type)
		{
		case EmDamageType::Physical:
			return "Physical";
		case EmDamageType::Magical:
			return "Magical";
		default:
			return "UnDefine";
		}
	}

	struct Damage
	{
	public: EmDamageType Type = EmDamageType::Physical;
	public: int Value = 0;
	};

	enum class EmSkillRange
	{
		Single,
		All,
	};

	class Creature;
	class IBattleInfo;
	class BattleEntities;
	class ISkill : public IEffect
	{
	public: enum class EmType
	{
		Passive,
		ActiveEffect,
		ActiveAttack
	};

	protected: std::weak_ptr<Creature> master;
	public: void SetMaster(std::shared_ptr<Creature> master) { this->master = master; }

	public: const EmSkill SkillType;
	public: std::string Name() const { return SkillToString(this->SkillType); }

	private: const EmType type;
	public: EmType Type() const { return type; }

	private: const int coolDown;
	public: int CoolDown() const { return coolDown; }
	private: int currentCoolDowning = 0;
	public: int CurrentCoolDowning() const { return currentCoolDowning; }
	public: void SetCurrentCoolDowning(int coolDowning) { this->currentCoolDowning = coolDowning; }

	public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo) = 0;
	public: virtual std::string Description() const = 0;

	protected: ISkill(std::shared_ptr<Creature> master, EmType type, EmSkill skillType, int coolDown) :
		master(master), type(type), SkillType(skillType), coolDown(coolDown)
	{

	}

	protected: inline std::shared_ptr<ISkill> ToISkill()
	{
		return std::static_pointer_cast<ISkill>(shared_from_this());
	}
	};

	enum class EmTargetSelectCreatureFlag
	{
		Self = 1 << 1,
		Enemy = 1 << 2,
		Teammate = 1 << 3,
	};

	EmTargetSelectCreatureFlag operator|(EmTargetSelectCreatureFlag a, EmTargetSelectCreatureFlag b);

	class ActiveSkill : public ISkill
	{
	public: class TargetSelectCreature : protected Util::BitFlags<EmTargetSelectCreatureFlag>
	{
	public: TargetSelectCreature(EmTargetSelectCreatureFlag flag) : BitFlags(flag)
	{}

	public: inline EmTargetSelectCreatureFlag Targets() const { return this->flags; }
	public: inline bool HasTarget(EmTargetSelectCreatureFlag flag) const { return this->hasFlag(flag); }
	};
	public: const TargetSelectCreature TargetCreatureFlags;
	public: EmSkillRange Range = EmSkillRange::Single;

	protected: int rollAmount = 0;
	public: int RollAmount() const { return rollAmount; }

	public: virtual double RollProbability() const = 0;
	public: virtual int Roll(int focus) const;
	public: virtual std::shared_ptr<IBattleInfo> BattleInfo(std::vector<std::shared_ptr<Creature>> targets, std::shared_ptr<BattleEntities> battleEntities, int focus) = 0;

	protected: ActiveSkill(std::shared_ptr<Creature> master, EmSkill skillType,
		int coolDown, int rollAmount,
		const TargetSelectCreature& targets,
		EmSkillRange range,
		EmType type = EmType::ActiveEffect)
		:
		ISkill(master, type, skillType, coolDown),
		rollAmount(rollAmount),
		TargetCreatureFlags(targets),
		Range(range)
	{

	}
	};

	class ActiveAttack : public ActiveSkill
	{
	protected: EmDamageType damageType;
	public: EmDamageType DamageType() const { return damageType; }

	private: double damageRate = 1.0;
	public: virtual Damage Damage(int roll) const;

	public: virtual std::shared_ptr<IBattleInfo> BattleInfo(std::vector<std::shared_ptr<Creature>> targets, std::shared_ptr<BattleEntities> battleEntities, int focus) override;

	public: virtual void Invoke(EmGameInvokeEvent gameInvokeEvent, std::shared_ptr<IBattleInfo> battleInfo) override;

	protected: ActiveAttack(
		std::shared_ptr<Creature> master,
		EmSkill skillType, int coolDown,
		int rollAmount, EmDamageType damageType,
		double damageRate, const TargetSelectCreature& targets, EmSkillRange range)
		:
		ActiveSkill(master, skillType, coolDown, rollAmount, targets, range, EmType::ActiveAttack), damageType(damageType), damageRate(damageRate)
	{

	}
	};

	class PassiveSkill : public ISkill
	{
	protected: PassiveSkill(std::shared_ptr<Creature> master, EmSkill skillType, int coolDown = 0) : ISkill(master, EmType::Passive, skillType, coolDown)
	{

	}
	};
}
#endif // !ISkill_H