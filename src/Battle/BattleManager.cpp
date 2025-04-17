#include "BattleManager.h"

#include "../Manager/GameStateController.hpp"
#include "../Util/LoDash/LoDash.hpp"

namespace FTK
{
	auto& gameStateController = GameStateController::Instance();

	BattleManager::BattleManager(std::shared_ptr<BattleEntities> battleEntities)
	{
		this->battleEntities = battleEntities;

		std::vector<std::shared_ptr<Creature>> creatures;
		for (const auto& player : this->battleEntities->Players)
		{
			for (auto& skill : player->Skills())
			{
				skill->SetCurrentCoolDowning(0);
			}
			creatures.push_back(player);
		}
		for (const auto& enemy : this->battleEntities->Enemys)
		{
			for (auto& skill : enemy->Skills())
			{
				skill->SetCurrentCoolDowning(0);
			}
			creatures.push_back(enemy);
		}

		this->processController = std::make_shared<ProcessController>(creatures);
		this->processController->OnCreatureTurnStart.Register(
			[&](std::shared_ptr<Creature> creature)
			{
				// Skill cooldown
				for (auto& skill : creature->Skills())
				{
					if (skill->CurrentCoolDowning() > 0)
					{
						skill->SetCurrentCoolDowning(skill->CurrentCoolDowning() - 1);
					}
				}

				// handle buff when creature turn start
				for (const auto& buff : creature->Buffs())
				{
					buff->Invoke(EmGameInvokeEvent::Battle_TurnStart, nullptr);
				}
				bool skipAction = LoDash::Any(creature->Buffs(),
					[](const std::shared_ptr<IBuff>& buff)
					{
						return buff->Type == EmBuff::Dizziness;
					}
				);
				skipAction &= creature->IsAlive();

				// reduce buff duration
				for (auto buff : creature->Buffs())
				{
					buff->ReduceDuration();
					if (buff->CurrentDuration() <= 0)
					{
						creature->RemoveBuff(buff);
					}
				}

				if (skipAction)
				{
					gameStateController.AddState(EmGameState::BattleWaitingConfirm);

					this->Event_OnBattleResult.Invoke(
						std::make_shared<IBattleInfo>(
							IBattleInfo::EmType::Skip,
							creature,
							std::vector<std::shared_ptr<Creature>>(),
							this->battleEntities,
							nullptr
						)
					);

					this->Process();
					return;
				}

				this->Event_WaitingCreatureSelectAction.Invoke(creature);
			}
		);
		this->processController->OnCreatureTurnEnd.Register(
			[&](std::shared_ptr<Creature> creature)
			{

			}
		);
	}

	void BattleManager::Battle(std::shared_ptr<ActiveBattleInfo> battleInfo)
	{
		auto master = battleInfo->Master.lock();

		// process EmState::CreatureTurn
		this->processController->Process();

		// invoke skill
		battleInfo->ActiveSkill->Invoke(EmGameInvokeEvent::Battle_UsingSkill, battleInfo);

		if (battleInfo->Type == IBattleInfo::EmType::Espace)
		{
			if (auto espaceInfo = std::dynamic_pointer_cast<EffectBattleInfo>(battleInfo); !espaceInfo->Fail)
			{
				this->processController->CreatureLeave(espaceInfo->Master.lock()->ID());
			}
		}

		// handle additional event
		if (battleInfo->AdditionalBattleInfo.size() != 0)
		{
			for (const auto& additional : battleInfo->AdditionalBattleInfo)
			{
				auto additionalDamageBattleInfo = std::dynamic_pointer_cast<AdditionalDamageBattleInfo>(additional);
				if (additionalDamageBattleInfo == nullptr)
					continue;

				for (std::size_t targetIndex = 0; targetIndex < additionalDamageBattleInfo->Targets.size(); targetIndex++)
				{
					auto target = additionalDamageBattleInfo->Targets[targetIndex];
					const auto& targetDefendDamage = additionalDamageBattleInfo->TargetDefendDamages[targetIndex];
					if (targetDefendDamage.Fail)
					{
						continue;
					}

					int damage = targetDefendDamage.Damage();
					additionalDamageBattleInfo->HurtDamages.push_back(damage);
					target->Hurt(damage);

					for (const auto& skill : target->Skills())
					{
						skill->Invoke(EmGameInvokeEvent::Battle_BeHurt, additional);
					}
					for (const auto& skill : additionalDamageBattleInfo->Master.lock()->Skills())
					{
						skill->Invoke(EmGameInvokeEvent::Battle_CauseHarm, additional);
					}
				}
			}
		}

		battleInfo->Master.lock()->UseFocus(battleInfo->UseFocus);
		battleInfo->UseSkill->SetCurrentCoolDowning(battleInfo->UseSkill->CoolDown());

		for (auto skill : battleInfo->InvokeSkills)
		{
			skill->SetCurrentCoolDowning(skill->CoolDown());
		}

		this->Event_OnBattleResult.Invoke(battleInfo);
	}

	void BattleManager::Process()
	{
		if (this->processController->State() == ProcessController::EmState::BattleEnd)
			return;

		if (this->processController->State() == ProcessController::EmState::CreatureTurnEnd)
		{
			// process EmState::CreatureTurnEnd
			this->processController->Process();

			// check whether contine battle 
			bool continueBattle = LoDash::Any(this->battleEntities->Enemys,
				[](const std::shared_ptr<Creature>& enemy)
				{
					return enemy->IsAlive();
				}
			);
			continueBattle &= LoDash::Any(this->battleEntities->Players,
				[this](const std::shared_ptr<Creature>& player)
				{
					return player->IsAlive() && !this->processController->CreatureIsLeave(player->ID());
				}
			);

			if (continueBattle == false)
			{
				// remove all buffs
				for (const auto& player : this->battleEntities->Players)
				{
					player->ClearBuff();
				}
				for (const auto& enemy : this->battleEntities->Enemys)
				{
					enemy->ClearBuff();
				}
			}

			this->processController->SetState(
				continueBattle
				? ProcessController::EmState::CreatureTurnStart
				: ProcessController::EmState::BattleEnd
			);
		}
		this->processController->Process();
	}
}