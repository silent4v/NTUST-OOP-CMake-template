#ifndef ProcessController_H
#define ProcessController_H

#include <algorithm>
#include <limits>

#include "../Entity/Creature.h"
#include "../Util/Event.hpp"
#include "../Util/LoDash/LoDash.hpp"

namespace FTK
{
	namespace Battle
	{
		class ProcessController
		{
		public: enum class EmState
		{
			BattleStart,
			CreatureTurnStart,
			CreatureTurn,
			CreatureTurnEnd,
			BattleEnd
		};

		private: EmState state = EmState::BattleStart;
		public: EmState State() const { return this->state; }
		public: void SetState(const EmState& state) { this->state = state; }
		private: std::vector<std::shared_ptr<Creature>> creatures;

		private: std::set<int> leaveBattleCreatureIDs;
		public: void CreatureLeave(int id)
		{
			this->leaveBattleCreatureIDs.insert(id);
		}
		public: bool CreatureIsLeave(int id) const
		{
			return this->leaveBattleCreatureIDs.find(id) != this->leaveBattleCreatureIDs.end();
		}
		private: std::vector<int> creatureActionCounters;
		private: std::vector<int> creatureSpeeds;
		private: std::shared_ptr<Creature> currentCreature = nullptr;
		public: inline std::shared_ptr<Creature> CurrentCreature() const { return this->currentCreature; }

		public: void ComputeCreatureSpeeds()
		{
			this->creatureSpeeds = LoDash::Map(this->creatures, [this](const std::size_t& index, const std::shared_ptr<Creature>& creature)
				{
					return static_cast<int>(
						std::floor(
							static_cast<double>(this->creatureActionCounters[index] + 1) /
							static_cast<double>(creature->CurrentAttribute().Speed) *
							100.
						));
				}
			);
		}
		public: ProcessController(const std::vector<std::shared_ptr<Creature>>& creatures) : creatures(creatures)
		{
			for (const auto& creature : creatures)
				this->creatureActionCounters.push_back(0);
			this->ComputeCreatureSpeeds();
		}

#pragma region Event

		public: Util::Event<void, EmState> OnStateChange;
		public: Util::Event<void, std::shared_ptr<Creature>> OnCreatureTurnStart;
		public: Util::Event<void, std::shared_ptr<Creature>> OnCreatureTurnEnd;

#pragma endregion

		private: int selectNextCreatureIndex()
		{
			std::vector<std::shared_ptr<Creature>> nextCreatures;
			int minSpeed = (std::numeric_limits<int>::max)();
			for (int i = 0; i < this->creatures.size(); i++)
			{
				if (!this->creatures[i]->IsAlive())
					continue;
				if (this->CreatureIsLeave(this->creatures[i]->ID()))
					continue;

				if (this->creatureSpeeds[i] < minSpeed)
				{
					nextCreatures.clear();
					nextCreatures.push_back(this->creatures[i]);
					minSpeed = this->creatureSpeeds[i];
				}
				else if (this->creatureSpeeds[i] == minSpeed)
				{
					nextCreatures.push_back(this->creatures[i]);
				}
			}

			// If the priority is the same, then compare in order
			//	- Current player attribute speed
			//	- Current physical + magic attack sum
			//	- Current physical + magic defense sum
			//	- Maximum HP
			// with higher values acts first
			std::sort(nextCreatures.begin(), nextCreatures.end(), [](const std::shared_ptr<Creature>& a, const std::shared_ptr<Creature>& b)
				{
					if (a->CurrentAttribute().Speed != b->CurrentAttribute().Speed)
						return a->CurrentAttribute().Speed > b->CurrentAttribute().Speed;
					if (a->CurrentAttribute().PhysicalAttack + a->CurrentAttribute().MagicalAttack != b->CurrentAttribute().PhysicalAttack + b->CurrentAttribute().MagicalAttack)
						return a->CurrentAttribute().PhysicalAttack + a->CurrentAttribute().MagicalAttack > b->CurrentAttribute().PhysicalAttack + b->CurrentAttribute().MagicalAttack;
					if (a->CurrentAttribute().PhysicalDefense + a->CurrentAttribute().MagicalDefense != b->CurrentAttribute().PhysicalDefense + b->CurrentAttribute().MagicalDefense)
						return a->CurrentAttribute().PhysicalDefense + a->CurrentAttribute().MagicalDefense > b->CurrentAttribute().PhysicalDefense + b->CurrentAttribute().MagicalDefense;
					return a->ModifyAttribute().HP > b->ModifyAttribute().HP;
				}
			);

			for (int i = 0; i < this->creatures.size(); i++)
			{
				if (this->creatures[i] == nextCreatures[0])
				{
					return i;
				}
			}

			return -1;
		}

		private: void battleStart()
		{
			this->state = EmState::CreatureTurnStart;

			this->Process();
		}
		private: void creatureTurnStart()
		{
			this->state = EmState::CreatureTurn;

			// Select which creature to action
			this->ComputeCreatureSpeeds();
			auto nextCreatureIndex = this->selectNextCreatureIndex();
			this->currentCreature = this->creatures[nextCreatureIndex];
			this->creatureActionCounters[nextCreatureIndex]++;

			this->OnCreatureTurnStart.Invoke(this->creatures[nextCreatureIndex]);
		}
		private: void creatureTurn()
		{
			this->state = EmState::CreatureTurnEnd;
		}
		private: void creatureTurnEnd()
		{
			this->OnCreatureTurnEnd.Invoke(this->currentCreature);
			this->currentCreature = nullptr;
		}
		private: void battleEnd()
		{
			this->currentCreature = nullptr;
		}

		public: void Process()
		{
			switch (this->state)
			{
			case EmState::BattleStart:
				this->battleStart();
				break;
			case EmState::CreatureTurnStart:
				this->creatureTurnStart();
				break;
			case EmState::CreatureTurn:
				this->creatureTurn();
				break;
			case EmState::CreatureTurnEnd:
				this->creatureTurnEnd();
				break;
			case EmState::BattleEnd:
				this->battleEnd();
				break;
			default:
				return;
			}

			this->OnStateChange.Invoke(this->state);
		}
		};
	}
}
#endif // !ProcessController_H