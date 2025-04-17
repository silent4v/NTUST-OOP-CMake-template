#ifndef IBattleInfo_H
#define IBattleInfo_H

#include <vector>

#include "../Entity/Player.hpp"
#include "../Skill/ISkill.h"

namespace FTK
{
	struct BattleEntities
	{
	public: std::vector<std::shared_ptr<Player>> Players;
	public: std::vector<std::shared_ptr<Creature>> Enemys;

	public: BattleEntities(const std::vector<std::shared_ptr<Player>>& players, const std::vector<std::shared_ptr<Creature>>& enemys)
		: Players(players), Enemys(enemys)
	{

	}
	public: BattleEntities(const BattleEntities& battleEntities)
		: Players(battleEntities.Players), Enemys(battleEntities.Enemys)
	{

	}
	};

	class IBattleInfo : public std::enable_shared_from_this<IBattleInfo>
	{
	public: enum class EmType
	{
		AttackInfo,
		HealInfo,
		AdditionalDamageInfo,
		EffectInfo,
		Espace,
		Skip,
	};

	public: EmType Type;
	public: std::weak_ptr<FTK::Creature> Master;
	public: std::vector<std::shared_ptr<FTK::Creature>> Targets;
	public: std::shared_ptr<ISkill> UseSkill;
	public: std::shared_ptr<FTK::BattleEntities> BattleEntities;

	public: IBattleInfo(
		EmType type,
		std::weak_ptr<FTK::Creature> master,
		std::vector<std::shared_ptr<FTK::Creature>> targets,
		std::shared_ptr<FTK::BattleEntities> battleEntities,
		std::shared_ptr<ISkill> useSkill)
		:
		Type(type), Master(master), Targets(targets), UseSkill(useSkill),
		BattleEntities(battleEntities)
	{
	}

	public: std::vector<std::shared_ptr<IBattleInfo>> AdditionalBattleInfo;
	public: std::vector<std::shared_ptr<ISkill>> InvokeSkills;

	public: virtual ~IBattleInfo() = default;
	};

	class ActiveBattleInfo : public IBattleInfo
	{
	public: int UseFocus = 0;
	protected: int roll = 0;
	public: inline int Roll() const { return this->roll; }

	public: std::shared_ptr<FTK::ActiveSkill> ActiveSkill = nullptr;

	public: ActiveBattleInfo(
		EmType type,
		std::weak_ptr<FTK::Creature> master,
		std::vector<std::shared_ptr<FTK::Creature>> targets,
		std::shared_ptr<FTK::BattleEntities> battleEntities,
		std::shared_ptr<FTK::ActiveSkill> useSkill,
		int useFocus);
	};

	struct AttackInfo
	{
	public: Damage SkillDamage;
	public: Damage TargetDefend;
	public: bool Fail = false;
	public: double DamageReductionRate = .0;
	public: int Damage() const
	{
		double damageReductionRate = this->DamageReductionRate;
		damageReductionRate += static_cast<double>(TargetDefend.Value) / static_cast<double>(TargetDefend.Value + 50);

		double damageRate = 1. - damageReductionRate;
		if (damageRate < 0.)
			damageRate = 0.;
		int damgae = this->SkillDamage.Value * damageRate;

		if (damgae < 1)
			damgae = 1;
		return damgae;
	}
	};

	class AttackBattleInfo : public ActiveBattleInfo
	{
	public: int TotalDamage = 0;
	public: std::shared_ptr<ActiveAttack> ActiveAttack = nullptr;
	public: std::vector<AttackInfo> TargetDefendDamages;

	public: std::vector<int> HurtDamages;

	public: AttackBattleInfo(
		std::weak_ptr<FTK::Creature> master, std::vector<std::shared_ptr<FTK::Creature>> targets,
		std::shared_ptr<FTK::BattleEntities> battleEntities,
		std::shared_ptr<FTK::ActiveSkill> useSkill, int useFocus,
		int debugRollAmount = -1);
	};

	class HealBattleInfo : public ActiveBattleInfo
	{
	public: int HealValue = 0;
	public: bool Fail = false;

	public: HealBattleInfo(
		std::weak_ptr<FTK::Creature> master, std::vector<std::shared_ptr<FTK::Creature>> targets,
		std::shared_ptr<FTK::BattleEntities> battleEntities,
		std::shared_ptr<FTK::ActiveSkill> useSkill, int useFocus);
	};

	class AdditionalDamageBattleInfo : public IBattleInfo
	{
	public: FTK::Damage Damage;
	public: std::vector<AttackInfo> TargetDefendDamages;
	public: std::vector<int> HurtDamages;

	public: AdditionalDamageBattleInfo(
		std::weak_ptr<FTK::Creature> master,
		std::vector<std::shared_ptr<FTK::Creature>> targets,
		std::shared_ptr<FTK::BattleEntities> battleEntities,
		std::shared_ptr<ISkill> skill,
		FTK::Damage damage);
	};

	class EffectBattleInfo : public ActiveBattleInfo
	{
	public: bool Fail = false;

	public: EffectBattleInfo(
		std::weak_ptr<FTK::Creature> master, std::vector<std::shared_ptr<FTK::Creature>> targets,
		std::shared_ptr<FTK::BattleEntities> battleEntities,
		std::shared_ptr<FTK::ActiveSkill> useSkill, int useFocus, EmType type = EmType::EffectInfo,
		int debugRollAmount = -1);
	};
}
#endif // !IBattleInfo_H