#ifndef AIAgent_H
#define AIAgent_H

#include "../Battle/BattleManager.h"
#include "../Battle/IBattleInfo.h"
#include "../Entity/Creature.h"
#include "../Util/LoDash/LoDash.hpp"

namespace FTK
{
	namespace ai
	{
		class AIAgent
		{
		protected: std::shared_ptr<Creature> master;
		protected: std::shared_ptr<BattleManager> battleManager;

		public: virtual std::shared_ptr<FTK::ActiveBattleInfo> Behavior() const
		{
			auto activeSkills = LoDash::Map(
				LoDash::Where(
					master->Skills(),
					[](const std::shared_ptr<ISkill>& skill)
					{
						if (skill->SkillType == EmSkill::Escape)
						{
							return false;
						}

						return
							skill->Type() == ISkill::EmType::ActiveAttack ||
							skill->Type() == ISkill::EmType::ActiveEffect;
					}
				),
				[](const std::shared_ptr<ISkill>& skill) -> std::shared_ptr<ActiveSkill>
				{
					return std::static_pointer_cast<ActiveSkill>(skill);
				}
			);
			auto skills = LoDash::Where(activeSkills,
				[](const std::shared_ptr<ActiveSkill>& skill)
				{
					return skill->CurrentCoolDowning() == 0;
				}
			);

			// ai select action
			int randomIndex = std::rand() % skills.size();
			auto skill = skills[randomIndex];

			auto targets = LoDash::Where(
				this->battleManager->battleEntities->Players,
				[&](const std::shared_ptr<Player>& player)
				{
					return player->IsAlive() && !this->battleManager->processController->CreatureIsLeave(player->ID());
				}
			);
			int targetIndex = std::rand() % targets.size();

			// TODO: if skill range is aoe
			return std::static_pointer_cast<ActiveBattleInfo>(
				skill->BattleInfo(
					std::vector<std::shared_ptr<Creature>>{ targets[targetIndex] },
					this->battleManager->battleEntities,
					0
				)
			);
		}
		public: AIAgent(std::shared_ptr<Creature> master, std::shared_ptr<BattleManager> battleManager)
			: master(master), battleManager(battleManager)
		{

		}
		};
	}
}
#endif // !AIAgent_H