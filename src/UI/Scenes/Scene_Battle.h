#ifndef Scene_Battle_H
#define Scene_Battle_H
#include <memory>
#include <string>

#include "../../ConsoleRender.hpp"
#include "../../Manager/EntityManager.hpp"

#include "IScene.h"

#include "../../Define/Color.h"
#include "../../Entity/Player.hpp"
#include "../../Battle/BattleManager.h"
#include "../Panel.h"
#include "../Pixel.hpp"

namespace FTK
{
	namespace UI
	{
		class Scene_Battle : public IScene
		{
		public: static const int Panel_ENTITYINFO_WIDTH = 40;
		public: static const int Panel_EMENYINFO_HEIGHT = 11;
		private: UIPanel panelEmenyInfos[3]{
			UI::UIPanel(Vector2D(5, 1), Panel_ENTITYINFO_WIDTH, Panel_EMENYINFO_HEIGHT),
			UI::UIPanel(Vector2D(50, 1), Panel_ENTITYINFO_WIDTH, Panel_EMENYINFO_HEIGHT),
			UI::UIPanel(Vector2D(95, 1), Panel_ENTITYINFO_WIDTH, Panel_EMENYINFO_HEIGHT)
		};
		public: static const int Panel_PLAYERINFO_HEIGHT = 16;
		private: UIPanel panelPlayerInfos[3]{
			UI::UIPanel(Vector2D(5, 35), Panel_ENTITYINFO_WIDTH, Panel_PLAYERINFO_HEIGHT),
			UI::UIPanel(Vector2D(50, 35), Panel_ENTITYINFO_WIDTH, Panel_PLAYERINFO_HEIGHT),
			UI::UIPanel(Vector2D(95, 35), Panel_ENTITYINFO_WIDTH, Panel_PLAYERINFO_HEIGHT)
		};

		public: static const int PANEL_BATTLEINFO_WIDTH = 140;
		public: static const int PANEL_BATTLEINFO_HEIGHT = 24;
		private: UIPanel panelBattleInfo = UI::UIPanel(
			Vector2D(0, 11), PANEL_BATTLEINFO_WIDTH, PANEL_BATTLEINFO_HEIGHT
		);

		private: std::shared_ptr<BattleManager> battleManager = nullptr;

		public: void SetBattleManager(std::shared_ptr<BattleManager> battleManager)
		{
			this->battleManager = battleManager;
		}

		private: void renderPlayerInfos()
		{
			for (int i = 0; i < 3; i++)
			{
				if (i >= this->battleManager->battleEntities->Players.size())
				{
					continue;
				}

				const auto& player = this->battleManager->battleEntities->Players[i];
				this->drawCreatureInfo(
					player,
					this->panelPlayerInfos[i],
					PixelColor(
						!this->battleManager->processController->CreatureIsLeave(player->ID()) && player->IsAlive()
						? EmColor::White
						: EmColor::DarkGray,
						EmColor::Black
					)
				);
			}
		}
		private: void renderEnemyInfos()
		{
			for (int i = 0; i < 3; i++)
			{
				if (i >= this->battleManager->battleEntities->Enemys.size())
				{
					continue;
				}

				const auto& enemy = this->battleManager->battleEntities->Enemys[i];
				this->drawCreatureInfo(
					enemy,
					this->panelEmenyInfos[i], PixelColor(
						!this->battleManager->processController->CreatureIsLeave(enemy->ID()) && enemy->IsAlive()
						? EmColor::Red
						: EmColor::DarkGray,
						EmColor::Black
					)
				);
			}
		}

#pragma region BattleInfoConsle

		public: class IBattleInfoConsole
		{
		public: virtual std::vector<DrawInfo> ToString() const = 0;
		};

		public: class PlayerSelectSkill : public IBattleInfoConsole
		{
		public: std::shared_ptr<Player> Player = nullptr;
		public: std::vector<std::shared_ptr<ActiveSkill>> Skills;
		public: int SelectIndex = 0;

		public: PlayerSelectSkill(std::shared_ptr<FTK::Player> player)
			: Player(player)
		{
			this->Skills =
				LoDash::Map(
					LoDash::Where(player->Skills(),
						[](const std::shared_ptr<ISkill>& skill) -> bool
						{
							return skill->Type() == ISkill::EmType::ActiveAttack ||
								skill->Type() == ISkill::EmType::ActiveEffect;
						}
					),
					[](const std::shared_ptr<ISkill>& skill) -> std::shared_ptr<ActiveSkill>
						{
							return std::static_pointer_cast<ActiveSkill>(skill);
						}
						);
		}

		public: std::shared_ptr<ActiveSkill> GetSelectSkill() const
		{
			if (this->SelectIndex >= this->Skills.size())
			{
				return nullptr;
			}

			return this->Skills[this->SelectIndex];
		}
		private: void activeAttackSkillInfo(std::vector<DrawInfo>& drawInfos, std::shared_ptr<ActiveAttack> activeAttack, Vector2D descriptionOffset) const
		{
			// show roll amount
			drawInfos.push_back(DrawInfo{
				"Roll Amount: " + std::to_string(activeAttack->RollAmount()),
				PixelColor(EmColor::White, EmColor::Black),
				descriptionOffset
				}
			);

			descriptionOffset.Y += 1;
			// show roll probability
			int probability = activeAttack->RollProbability() * 100;
			drawInfos.push_back(DrawInfo{
				"Roll Probability: " + std::to_string(probability) + "%",
				PixelColor(EmColor::White, EmColor::Black),
				descriptionOffset
				}
			);

			descriptionOffset.Y += 1;
			// show damage type
			drawInfos.push_back(DrawInfo{
				"Damage Type: " + DamageTypeToString(activeAttack->DamageType()),
				PixelColor(EmColor::White, EmColor::Black),
				descriptionOffset
				}
			);

			descriptionOffset.Y += 1;
			// show damage range
			if (activeAttack->RollAmount() == 1)
			{
				drawInfos.push_back(DrawInfo{
					"Damage: " + std::to_string(activeAttack->Damage(1).Value),
					PixelColor(EmColor::White, EmColor::Black),
					descriptionOffset
					}
				);
			}
			else
			{
				int maxDamage = activeAttack->Damage(activeAttack->RollAmount()).Value;
				int minDamage = activeAttack->Damage(1).Value;
				drawInfos.push_back(DrawInfo{
					"Damage Range: " + std::to_string(minDamage) + " ~ " + std::to_string(maxDamage),
					PixelColor(EmColor::White, EmColor::Black),
					descriptionOffset
					}
				);
			}
			descriptionOffset.Y += 1;
			// cool down
			drawInfos.push_back(DrawInfo{
				"Cool Down: " + std::to_string(activeAttack->CurrentCoolDowning()) + "/" + std::to_string(activeAttack->CoolDown()),
				PixelColor(EmColor::White, EmColor::Black),
				descriptionOffset
				}
			);
		}
		private: void activeEffectSkillInfo(std::vector<DrawInfo>& drawInfos, std::shared_ptr<ActiveSkill> activeAttack, Vector2D descriptionOffset) const
		{
			// show roll amount
			drawInfos.push_back(DrawInfo{
				"Roll Amount: " + std::to_string(activeAttack->RollAmount()),
				PixelColor(EmColor::White, EmColor::Black),
				descriptionOffset
				}
			);

			descriptionOffset.Y += 1;
			// show roll probability
			int probability = activeAttack->RollProbability() * 100;
			drawInfos.push_back(DrawInfo{
				"Roll Probability: " + std::to_string(probability) + "%",
				PixelColor(EmColor::White, EmColor::Black),
				descriptionOffset
				}
			);

			descriptionOffset.Y += 1;
			// cool down
			drawInfos.push_back(DrawInfo{
				"Cool Down: " + std::to_string(activeAttack->CurrentCoolDowning()) + "/" + std::to_string(activeAttack->CoolDown()),
				PixelColor(EmColor::White, EmColor::Black),
				descriptionOffset
				}
			);
		}
		public: virtual std::vector<DrawInfo> ToString() const override
		{
			std::vector<DrawInfo> drawInfos;

			Vector2D offset = Vector2D(2, 2);
			// Draw player name
			drawInfos.push_back(DrawInfo{
				"Name: " + this->Player->Name(),
				PixelColor(EmColor::White, EmColor::Black),
				offset
				}
			);
			offset.Y += 2;

			auto startIndex = this->SelectIndex;
			if (startIndex + 8 > this->Skills.size())
			{
				startIndex = this->Skills.size() - 8;
				if (startIndex < 0)
				{
					startIndex = 0;
				}
			}

			for (int i = startIndex; i < this->Skills.size() && i < startIndex + 8; i++)
			{
				if (i >= this->Skills.size())
				{
					break;
				}


				if (this->SelectIndex == i)
				{
					drawInfos.push_back(DrawInfo{
						"->",
						PixelColor(EmColor::Yellow, EmColor::Black),
						offset + Vector2D(0, i)
						}
					);
				}

				std::string str = this->Skills[i]->Name();
				drawInfos.push_back(DrawInfo{
					this->Skills[i]->Name(),
					PixelColor(
						this->Skills[i]->CurrentCoolDowning() > 0
							? EmColor::DarkGray
							: EmColor::White,
						EmColor::Black
					),
					offset + Vector2D(3, i)
					}
				);
			}

			// show skill description
			auto descriptionOffset = Vector2D(40, 4);
			drawInfos.push_back(DrawInfo{
				"Description:",
				PixelColor(EmColor::White, EmColor::Black),
				descriptionOffset
				}
			);

			descriptionOffset.X += 2;
			descriptionOffset.Y += 1;
			auto selectSkill = this->Skills[this->SelectIndex];
			switch (selectSkill->Type())
			{
			case ISkill::EmType::ActiveAttack:
			{
				this->activeAttackSkillInfo(drawInfos, std::static_pointer_cast<ActiveAttack>(selectSkill), descriptionOffset);
			}
			break;
			case ISkill::EmType::ActiveEffect:
			{
				this->activeEffectSkillInfo(drawInfos, std::static_pointer_cast<ActiveSkill>(selectSkill), descriptionOffset);
			}
			break;
			default:
				break;
			}

			// show helper
			auto helperOffset = Vector2D(80, 4);
			drawInfos.push_back(DrawInfo{
				"Helper:",
				PixelColor(EmColor::White, EmColor::Black),
				helperOffset
				}
			);
			helperOffset.X += 2;
			helperOffset.Y += 1;
			drawInfos.push_back(DrawInfo{
				"Arrow Key(Up/Down): Choose skill",
				PixelColor(EmColor::White, EmColor::Black),
				helperOffset
				}
			);
			helperOffset.Y += 1;
			drawInfos.push_back(DrawInfo{
				"Enter: Using item",
				PixelColor(EmColor::White, EmColor::Black),
				helperOffset
				}
			);
			helperOffset.Y += 1;
			drawInfos.push_back(DrawInfo{
				"I: Open bag",
				PixelColor(EmColor::White, EmColor::Black),
				helperOffset
				}
			);

			return drawInfos;
		}
		};

		public: class SelectItem : public IBattleInfoConsole
		{
		public: std::vector<std::shared_ptr<Item>> ItemList;
		public: int SelectIndex = 0;
		public: std::map<Item::EmType, int> BagItems;
		public: SelectItem(const std::map<Item::EmType, int>& bagItems)
			: BagItems(bagItems)
		{
			this->ItemList = LoDash::Where(
				LoDash::Map(
					LoDash::Keys(bagItems),
					[](const Item::EmType& type) -> std::shared_ptr<Item>
					{
						return NewItem(type);
					}
				),
				[](const std::shared_ptr<Item>& item) -> bool
					{
						return item->CanUseWhenBattle;
					}
					);
		}
		public: virtual std::vector<DrawInfo> ToString() const override
		{
			std::vector<DrawInfo> drawInfos;

			Vector2D offset = Vector2D(2, 2);
			// show item list
			drawInfos.push_back(DrawInfo{
				"Bag",
				PixelColor(EmColor::White, EmColor::Black),
				offset
				}
			);
			offset.Y += 2;

			if (this->ItemList.size() == 0)
			{
				drawInfos.push_back(DrawInfo{
					"No item",
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);
			}
			else
			{
				auto startIndex = this->SelectIndex;
				if (startIndex + 8 > this->ItemList.size())
				{
					startIndex = this->ItemList.size() - 8;
					if (startIndex < 0)
					{
						startIndex = 0;
					}
				}

				for (int i = startIndex; i < this->ItemList.size() && i < startIndex + 8; i++)
				{
					if (i >= this->ItemList.size())
					{
						break;
					}


					if (this->SelectIndex == i)
					{
						drawInfos.push_back(DrawInfo{
							"->",
							PixelColor(EmColor::Yellow, EmColor::Black),
							offset + Vector2D(0, i)
							}
						);
					}

					auto item = ItemList[i];
					std::string itemInfo =
						ItemList[i]->Name + ": " +
						std::to_string(this->BagItems.at(ItemList[i]->Type));

					if (Item::IsEquipment(item->Type))
					{
						const auto& alreadyEquippedItems = PlayerManager::Instance().AlreadyEquippedItems();
						if (auto itor = alreadyEquippedItems.find(item->Type); itor != alreadyEquippedItems.end())
						{
							const auto& [item, amount] = *itor;
							itemInfo += ", E*" + std::to_string(amount);
						}
					}
					drawInfos.push_back(DrawInfo{
						itemInfo,
						PixelColor(EmColor::White, EmColor::Black),
						offset + Vector2D{3, i}
						}
					);
				}

				// show item detail
				auto descriptionOffset = Vector2D(40, 4);
				drawInfos.push_back(DrawInfo{
					"Description:",
					PixelColor(EmColor::White, EmColor::Black),
					descriptionOffset
					}
				);

				descriptionOffset.X += 2;
				descriptionOffset.Y += 1;
				auto selectItem = this->ItemList[this->SelectIndex];
				drawInfos.push_back(DrawInfo{
					selectItem->Description,
					PixelColor(EmColor::White, EmColor::Black),
					descriptionOffset
					}
				);
			}

			return drawInfos;
		}
		};

		public: class SelectTarget : public IBattleInfoConsole
		{
		public: std::shared_ptr<BattleEntities> BattleEntities = nullptr;
		public: std::shared_ptr<ActiveSkill> Skill = nullptr;
		public: std::shared_ptr<Player> Player = nullptr;

			  /// <summary>
			  ///	0~2: Enemy
			  /// 	3~5: Player
			  /// </summary>
		public: int SelectIndex = 0;
		public: EmSkillRange SkillRange = EmSkillRange::Single;

		public: bool CanSelect(int selectIndex) const
		{
			int selfIndex = 3;
			for (const auto& player : this->BattleEntities->Players)
			{
				if (player == this->Player)
				{
					break;
				}
				selfIndex++;
			}

			if (selectIndex < 3)
			{
				if (selectIndex >= this->BattleEntities->Enemys.size())
				{
					return false;
				}

				if (this->Skill->TargetCreatureFlags.HasTarget(EmTargetSelectCreatureFlag::Enemy))
				{
					return this->BattleEntities->Enemys[selectIndex]->IsAlive();
				}
			}
			else
			{
				if (this->Skill->TargetCreatureFlags.HasTarget(EmTargetSelectCreatureFlag::Teammate))
				{
					if (!this->Skill->TargetCreatureFlags.HasTarget(EmTargetSelectCreatureFlag::Self))
					{
						return selectIndex != selfIndex;
					}

					if (this->BattleEntities->Players.size() <= selectIndex - 3)
					{
						return false;
					}

					return this->BattleEntities->Players[selectIndex - 3]->IsAlive();
				}
				else if (this->Skill->TargetCreatureFlags.HasTarget(EmTargetSelectCreatureFlag::Self))
				{
					return selectIndex == selfIndex;
				}
			}

			return false;
		}
		public: SelectTarget(std::shared_ptr<FTK::BattleEntities> battleEntities, std::shared_ptr<ActiveSkill> skill, std::shared_ptr<FTK::Player> player)
			: BattleEntities(battleEntities), Skill(skill), Player(player)
		{
			this->SkillRange = skill->Range;

			for (int i = 0; i < 6; i++)
			{
				if (this->CanSelect(i))
				{
					this->SelectIndex = i;
					break;
				}
			}
		}

		public: std::vector<std::shared_ptr<Creature>> GetTargets() const
		{
			if (this->SkillRange == EmSkillRange::All)
			{
				if (this->Skill->TargetCreatureFlags.HasTarget(EmTargetSelectCreatureFlag::Enemy))
				{
					return this->BattleEntities->Enemys;
				}
				else
				{
					return LoDash::Map(this->BattleEntities->Players,
						[](const std::shared_ptr<FTK::Player>& player) -> std::shared_ptr<Creature>
						{
							return player;
						}
					);
				}
			}

			std::vector<std::shared_ptr<Creature>> targets;
			if (this->SelectIndex < 3)
			{
				targets.push_back(this->BattleEntities->Enemys[this->SelectIndex]);
			}
			else
			{
				targets.push_back(this->BattleEntities->Players[this->SelectIndex - 3]);
			}

			return targets;
		}

		public: virtual std::vector<DrawInfo> ToString() const override
		{
			std::vector<DrawInfo> drawInfos;

			Vector2D offset = Vector2D(2, 2);
			drawInfos.push_back(DrawInfo{
				"Name: " + this->Player->Name(),
				PixelColor(EmColor::White, EmColor::Black),
				offset
				}
			);

			// show helper
			offset.Y += 1;
			drawInfos.push_back(DrawInfo{
				"Select Target, input right or left arrow key to change target",
				PixelColor(EmColor::White, EmColor::Black),
				offset
				}
			);
			offset.Y += 1;
			drawInfos.push_back(DrawInfo{
				"input enter to confirm",
				PixelColor(EmColor::White, EmColor::Black),
				offset
				}
			);

			return drawInfos;
		}
		};

		public: class UseFocus : public IBattleInfoConsole
		{
		public: std::shared_ptr<Player> Player = nullptr;
		public: std::shared_ptr<ActiveSkill> Skill = nullptr;
		public: int FocusAmount = 0;

		public: UseFocus(std::shared_ptr<FTK::Player> player, std::shared_ptr<ActiveSkill> skill)
			: Player(player), Skill(skill)
		{

		}

		public: virtual std::vector<DrawInfo> ToString() const override
		{
			std::vector<DrawInfo> drawInfos;

			Vector2D offset = Vector2D(2, 2);
			drawInfos.push_back(DrawInfo{
				"Name: " + this->Player->Name(),
				PixelColor(EmColor::White, EmColor::Black),
				offset
				}
			);

			// show use focus
			offset.Y += 2;
			int maxFocus = this->Player->ModifyAttribute().Focus;
			int remainFocus = this->Player->CurrentAttribute().Focus;
			{
				std::string focusTitle = "UseFocus: ";

				drawInfos.push_back(DrawInfo{
					focusTitle,
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);

				for (int i = 0; i < maxFocus; i++)
				{
					if (i < remainFocus)
					{
						drawInfos.push_back(DrawInfo{
							"*",
							PixelColor(i < this->FocusAmount ? EmColor::Yellow : EmColor::DarkGray, EmColor::Black),
							offset + Vector2D(focusTitle.size() + i, 0)
							}
						);
					}
					else
					{
						drawInfos.push_back(DrawInfo{
							"*",
							PixelColor(EmColor::Red, EmColor::Black),
							offset + Vector2D(focusTitle.size() + i, 0)
							}
						);
					}
				}
			}

			// show helper
			offset.Y += 1;
			drawInfos.push_back(DrawInfo{
				"Use Focus, input right or left arrow key to change focus amount",
				PixelColor(EmColor::White, EmColor::Black),
				offset
				}
			);
			offset.Y += 1;
			drawInfos.push_back(DrawInfo{
				"input enter to confirm",
				PixelColor(EmColor::White, EmColor::Black),
				offset
				}
			);

			return drawInfos;
		}
		};

		public: class BattleDetailConsole : public IBattleInfoConsole
		{
		public: int WaitingAmount = 1;

		private: std::vector<std::vector<DrawInfo>> details;
		private: void attackInfoDetails(std::shared_ptr<AttackBattleInfo> battleInfo)
		{
			std::vector<std::vector<DrawInfo>> result;

			std::vector<DrawInfo> drawInfos;
			{
				Vector2D offset = Vector2D(2, 2);
				drawInfos.push_back(DrawInfo{
					"Name: " + battleInfo->Master.lock()->Name(),
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);

				offset.Y += 1;
				drawInfos.push_back(DrawInfo{
					"Using skill: " + battleInfo->UseSkill->Name(),
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);

				offset.Y += 1;
				{
					std::string rollStr = "Roll: ";
					drawInfos.push_back(DrawInfo{
						"Roll: " + std::to_string(battleInfo->Roll()),
						PixelColor(EmColor::White, EmColor::Black),
						offset
						}
					);
					int totalRoll = battleInfo->ActiveAttack->RollAmount();
					int useFocus = battleInfo->UseFocus;
					int roll = battleInfo->Roll();
					for (int i = 0; i < useFocus; i++)
					{
						drawInfos.push_back(DrawInfo{
							"*",
							PixelColor(EmColor::Yellow, EmColor::Black),
							offset + Vector2D(rollStr.size() + i, 0)
							}
						);
					}
					for (int i = 0; i < roll - useFocus; i++)
					{
						drawInfos.push_back(DrawInfo{
							"*",
							PixelColor(EmColor::White, EmColor::Black),
							offset + Vector2D(rollStr.size() + useFocus + i, 0)
							}
						);
					}
					for (int i = 0; i < totalRoll - roll; i++)
					{
						drawInfos.push_back(DrawInfo{
							"*",
							PixelColor(EmColor::Red, EmColor::Black),
							offset + Vector2D(rollStr.size() + roll + i, 0)
							}
						);
					}
				}

				offset.Y += 2;
				drawInfos.push_back(DrawInfo{
					"DamageType: " + DamageTypeToString(battleInfo->ActiveAttack->DamageType()),
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);
				offset.X += 3;
				offset.Y += 1;
				for (int i = 0; i < battleInfo->Targets.size(); i++)
				{
					auto target = battleInfo->Targets[i];
					const auto& targetDamage = battleInfo->TargetDefendDamages[i];
					if (targetDamage.Fail)
					{
						drawInfos.push_back(DrawInfo{
							"Using skill fail",
							PixelColor(EmColor::Red, EmColor::Black),
							offset
							}
						);
						continue;
					}
					const auto& hurtDamage = battleInfo->HurtDamages[i];

					std::string content = "Inflict ";
					drawInfos.push_back(DrawInfo{
						content,
						PixelColor(EmColor::White, EmColor::Black),
						offset
						}
					);

					std::string damageStr = std::to_string(hurtDamage);
					drawInfos.push_back(DrawInfo{
						std::to_string(hurtDamage),
						PixelColor(EmColor::Red, EmColor::Black),
						offset + Vector2D(content.size(), 0)
						}
					);
					content += damageStr;
					drawInfos.push_back(DrawInfo{
						" points of damage to ",
						PixelColor(EmColor::White, EmColor::Black),
						offset + Vector2D(content.size(), 0)
						}
					);
					content += " points of damage to ";
					drawInfos.push_back(DrawInfo{
						target->Name(),
						PixelColor(
							target->Type() == Entity::EmType::Enemy
							? EmColor::Red
							: EmColor::Green,
							EmColor::Black
						),
						offset + Vector2D(content.size(), 0)
						}
					);

					offset.Y += 1;
				}

				offset.X -= 3;
				offset.Y += 2;
				drawInfos.push_back(DrawInfo{
					"Press any key to continue",
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);

				result.push_back(drawInfos);
			}

			this->details = result;
		}
		private: void effectInfoDetails(std::shared_ptr<EffectBattleInfo> battleInfo)
		{
			std::vector<std::vector<DrawInfo>> result;

			std::vector<DrawInfo> drawInfos;
			{
				Vector2D offset = Vector2D(2, 2);
				drawInfos.push_back(DrawInfo{
					"Name: " + battleInfo->Master.lock()->Name(),
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);

				offset.Y += 1;
				drawInfos.push_back(DrawInfo{
					"Using skill: " + battleInfo->UseSkill->Name(),
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);

				offset.Y += 1;
				{
					std::string rollStr = "Roll: ";
					drawInfos.push_back(DrawInfo{
						"Roll: " + std::to_string(battleInfo->Roll()),
						PixelColor(EmColor::White, EmColor::Black),
						offset
						}
					);
					int totalRoll = battleInfo->ActiveSkill->RollAmount();
					int useFocus = battleInfo->UseFocus;
					int roll = battleInfo->Roll();
					for (int i = 0; i < useFocus; i++)
					{
						drawInfos.push_back(DrawInfo{
							"*",
							PixelColor(EmColor::Yellow, EmColor::Black),
							offset + Vector2D(rollStr.size() + i, 0)
							}
						);
					}
					for (int i = 0; i < roll - useFocus; i++)
					{
						drawInfos.push_back(DrawInfo{
							"*",
							PixelColor(EmColor::White, EmColor::Black),
							offset + Vector2D(rollStr.size() + useFocus + i, 0)
							}
						);
					}
					for (int i = 0; i < totalRoll - roll; i++)
					{
						drawInfos.push_back(DrawInfo{
							"*",
							PixelColor(EmColor::Red, EmColor::Black),
							offset + Vector2D(rollStr.size() + roll + i, 0)
							}
						);
					}
				}

				offset.X += 3;
				offset.Y += 1;
				{
					drawInfos.push_back(DrawInfo{
						battleInfo->Fail
							? "Fail"
							: "Success",
						PixelColor(EmColor::White, EmColor::Black),
						offset
						}
					);
				}

				offset.X -= 3;
				offset.Y += 2;
				drawInfos.push_back(DrawInfo{
					"Press any key to continue",
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);

				result.push_back(drawInfos);
			}

			this->details = result;
		}
		private: void healInfoDetails(std::shared_ptr<HealBattleInfo> battleInfo)
		{
			std::vector<std::vector<DrawInfo>> result;

			std::vector<DrawInfo> drawInfos;
			{
				Vector2D offset = Vector2D(2, 2);
				drawInfos.push_back(DrawInfo{
					"Name: " + battleInfo->Master.lock()->Name(),
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);

				offset.Y += 1;
				drawInfos.push_back(DrawInfo{
					"Using skill: " + battleInfo->UseSkill->Name(),
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);

				offset.Y += 1;
				{
					std::string rollStr = "Roll: ";
					drawInfos.push_back(DrawInfo{
						"Roll: " + std::to_string(battleInfo->Roll()),
						PixelColor(EmColor::White, EmColor::Black),
						offset
						}
					);
					int totalRoll = battleInfo->ActiveSkill->RollAmount();
					int useFocus = battleInfo->UseFocus;
					int roll = battleInfo->Roll();
					for (int i = 0; i < useFocus; i++)
					{
						drawInfos.push_back(DrawInfo{
							"*",
							PixelColor(EmColor::Yellow, EmColor::Black),
							offset + Vector2D(rollStr.size() + i, 0)
							}
						);
					}
					for (int i = 0; i < roll - useFocus; i++)
					{
						drawInfos.push_back(DrawInfo{
							"*",
							PixelColor(EmColor::White, EmColor::Black),
							offset + Vector2D(rollStr.size() + useFocus + i, 0)
							}
						);
					}
					for (int i = 0; i < totalRoll - roll; i++)
					{
						drawInfos.push_back(DrawInfo{
							"*",
							PixelColor(EmColor::Red, EmColor::Black),
							offset + Vector2D(rollStr.size() + roll + i, 0)
							}
						);
					}
				}

				auto healInfo = std::static_pointer_cast<HealBattleInfo>(battleInfo);
				offset.X += 3;
				offset.Y += 1;
				for (int i = 0; i < battleInfo->Targets.size(); i++)
				{
					if (battleInfo->Fail)
					{
						drawInfos.push_back(DrawInfo{
							"Using skill fail",
							PixelColor(EmColor::Red, EmColor::Black),
							offset
							}
						);
						continue;
					}

					std::string content = "Heal ";
					drawInfos.push_back(DrawInfo{
						content,
						PixelColor(EmColor::White, EmColor::Black),
						offset
						}
					);

					std::string valueStr = std::to_string(healInfo->HealValue);
					drawInfos.push_back(DrawInfo{
						valueStr,
						PixelColor(EmColor::LightGreen, EmColor::Black),
						offset + Vector2D(content.size(), 0)
						}
					);
					content += valueStr;
					drawInfos.push_back(DrawInfo{
						" points to ",
						PixelColor(EmColor::White, EmColor::Black),
						offset + Vector2D(content.size(), 0)
						}
					);
					content += " points to ";
					drawInfos.push_back(DrawInfo{
						battleInfo->Targets[0]->Name(),
						PixelColor(
							battleInfo->Targets[0]->Type() == Entity::EmType::Enemy
							? EmColor::Red
							: EmColor::Green,
							EmColor::Black
						),
						offset + Vector2D(content.size(), 0)
						}
					);

					offset.Y += 1;
				}

				offset.X -= 3;
				offset.Y += 2;
				drawInfos.push_back(DrawInfo{
					"Press any key to continue",
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);

				result.push_back(drawInfos);
			}

			this->details = result;
		}
		private: void skipInfoDetails(std::shared_ptr<IBattleInfo> battleInfo)
		{
			std::vector<std::vector<DrawInfo>> result;

			std::vector<DrawInfo> drawInfos;
			{
				Vector2D offset = Vector2D(2, 2);
				drawInfos.push_back(DrawInfo{
					"Name: " + battleInfo->Master.lock()->Name(),
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);

				offset.Y += 1;
				drawInfos.push_back(DrawInfo{
					"Skip action",
					PixelColor(EmColor::White, EmColor::Black),
					offset
					}
				);

				result.push_back(drawInfos);
			}

			this->details = result;
		}
		public: BattleDetailConsole(std::shared_ptr<IBattleInfo> battleInfo)
		{
			if (battleInfo == nullptr)
			{
				return;
			}

			// TODO: heal info
			switch (battleInfo->Type)
			{
			case IBattleInfo::EmType::AttackInfo:
			{
				this->attackInfoDetails(std::static_pointer_cast<AttackBattleInfo>(battleInfo));
			}
			break;
			case IBattleInfo::EmType::Espace:
				[[fallthrough]];
			case IBattleInfo::EmType::EffectInfo:
			{
				this->effectInfoDetails(std::static_pointer_cast<EffectBattleInfo>(battleInfo));
			}
			break;
			case IBattleInfo::EmType::HealInfo:
			{
				this->healInfoDetails(std::static_pointer_cast<HealBattleInfo>(battleInfo));
			}
			break;
			case IBattleInfo::EmType::Skip:
			{
				this->skipInfoDetails(battleInfo);
			}
			break;
			default:
				break;
			}
		}

		public: virtual std::vector<DrawInfo> ToString() const override
		{
			auto index = this->WaitingAmount - 1;
			if (index >= this->details.size())
				return {};

			return this->details[index];
		}
		};

		public: class BattleInfoConsole
		{
		public: std::shared_ptr<IBattleInfoConsole> CurrentConsole = nullptr;
		public: std::shared_ptr<PlayerSelectSkill> PlayerSelectSkill = nullptr;
		public: std::shared_ptr<SelectTarget> SelectTarget = nullptr;
		public: std::shared_ptr<UseFocus> UseFocus = nullptr;
		public: std::shared_ptr<BattleDetailConsole> BattleDetailConsole = nullptr;
		public: std::shared_ptr<SelectItem> SelectItem = nullptr;

		public: inline void Reset()
		{
			this->CurrentConsole = nullptr;
			this->PlayerSelectSkill = nullptr;
			this->SelectTarget = nullptr;
			this->UseFocus = nullptr;
			this->BattleDetailConsole = nullptr;
		}

		public: inline void SetPlayerSelectSkill(std::shared_ptr<Scene_Battle::PlayerSelectSkill> playerSelectSkill)
		{
			this->CurrentConsole = playerSelectSkill;
			this->PlayerSelectSkill = playerSelectSkill;
		}
		public: inline void ChangePlayerSelectSkillIndex(int offset)
		{
			if (this->PlayerSelectSkill == nullptr)
				return;
			if (this->PlayerSelectSkill->SelectIndex + offset >= this->PlayerSelectSkill->Skills.size())
				return;
			if (this->PlayerSelectSkill->SelectIndex + offset < 0)
				return;

			this->PlayerSelectSkill->SelectIndex += offset;
		}

		public: inline void SetSelectItem(std::shared_ptr<Scene_Battle::SelectItem> selectItem)
		{
			this->CurrentConsole = selectItem;
			this->SelectItem = selectItem;
		}
		public: inline void ChangeSelectItemIndex(int offset)
		{
			if (this->SelectItem == nullptr)
				return;
			if (this->SelectItem->SelectIndex + offset >= this->SelectItem->ItemList.size())
				return;
			if (this->SelectItem->SelectIndex + offset < 0)
				return;

			this->SelectItem->SelectIndex += offset;
		}
		public: inline std::shared_ptr<Item> GetSelectedItem() const
		{
			if (this->SelectItem == nullptr)
				return nullptr;

			return this->SelectItem->ItemList[this->SelectItem->SelectIndex];
		}

		public: inline void SetSelectTarget(std::shared_ptr<Scene_Battle::SelectTarget> selectTarget)
		{
			this->CurrentConsole = selectTarget;
			this->SelectTarget = selectTarget;
		}
		public: inline void ChangeSelectTargetIndex(int offset)
		{
			if (offset == 0)
				return;

			if (this->SelectTarget == nullptr)
				return;
			if (this->SelectTarget->SelectIndex + offset >= 6)
				return;
			if (this->SelectTarget->SelectIndex + offset < 0)
				return;

			if (offset > 0)
			{
				for (int i = this->SelectTarget->SelectIndex + 1; i < 6; i++)
				{
					if (this->SelectTarget->CanSelect(i))
					{
						this->SelectTarget->SelectIndex = i;
						break;
					}
				}
			}
			else
			{
				for (int i = this->SelectTarget->SelectIndex - 1; i >= 0; i--)
				{
					if (this->SelectTarget->CanSelect(i))
					{
						this->SelectTarget->SelectIndex = i;
						break;
					}
				}
			}
		}

		public: inline void SetUseFocus(std::shared_ptr<Scene_Battle::UseFocus> useFocus)
		{
			this->CurrentConsole = useFocus;
			this->UseFocus = useFocus;
		}
		public: inline void ChangeUseFocusAmount(int offset)
		{
			if (this->UseFocus == nullptr)
				return;
			if (this->UseFocus->FocusAmount + offset > this->UseFocus->Player->CurrentAttribute().Focus)
				return;
			if (this->UseFocus->FocusAmount + offset < 0)
				return;
			if (this->UseFocus->FocusAmount + offset > this->UseFocus->Skill->RollAmount())
				return;

			this->UseFocus->FocusAmount += offset;
		}

		public: inline void SetBattleDetailConsole(std::shared_ptr<Scene_Battle::BattleDetailConsole> detailConsole)
		{
			this->CurrentConsole = detailConsole;
			this->BattleDetailConsole = detailConsole;
		}
		public: inline void NextDetailConsole()
		{
			if (this->BattleDetailConsole == nullptr)
				return;

			this->BattleDetailConsole->WaitingAmount--;
		}

		public: std::vector<DrawInfo> ToString() const
		{
			if (this->CurrentConsole == nullptr)
				return {};

			return this->CurrentConsole->ToString();
		}
		} BattleInfoConsole;

		private: void renderBattleInfo()
		{
			this->panelBattleInfo.ClearPanel();

			if (this->BattleInfoConsole.CurrentConsole == nullptr)
			{
				this->panelBattleInfo.DrawPanel("Battle Info", PixelColor(EmColor::White, EmColor::Black), true);
				return;
			}

			auto contents = this->BattleInfoConsole.ToString();
			if (contents.size() == 0)
			{
				return;
			}

			this->panelBattleInfo.DrawPanel(contents, false);
		}

#pragma endregion

		public: virtual void Awake() override
		{
			this->BattleInfoConsole.Reset();
		}

		private: void drawPanelEdge(UIPanel& panel, const EmColor& color = EmColor::Green)
		{
			panel.DrawEdge(PixelColor(color, EmColor::Black));
		}
		private: void focusCreaturePanelWhenSelectAsTarget()
		{
			for (int i = 0; i < 3; i++)
			{
				this->drawPanelEdge(this->panelPlayerInfos[i], EmColor::White);
				this->drawPanelEdge(this->panelEmenyInfos[i], EmColor::White);
			}

			if (this->BattleInfoConsole.CurrentConsole == nullptr)
			{
				return;
			}
			if (this->BattleInfoConsole.SelectTarget == nullptr)
			{
				return;
			}

			if (!(this->BattleInfoConsole.CurrentConsole == this->BattleInfoConsole.SelectTarget ||
				this->BattleInfoConsole.CurrentConsole == this->BattleInfoConsole.UseFocus))
			{
				return;
			}

			if (this->BattleInfoConsole.SelectTarget->SkillRange == EmSkillRange::All)
			{
				if (this->BattleInfoConsole.SelectTarget->Skill->TargetCreatureFlags.HasTarget(EmTargetSelectCreatureFlag::Enemy))
				{
					for (int i = 0; i < 3; i++)
					{
						this->drawPanelEdge(this->panelEmenyInfos[i]);
					}
				}
				else
				{
					for (int i = 0; i < 3; i++)
					{
						this->drawPanelEdge(this->panelPlayerInfos[i]);
					}
				}
				return;
			}

			auto selectIndex = this->BattleInfoConsole.SelectTarget->SelectIndex;
			if (selectIndex == -1)
				return;

			if (selectIndex < 3)
			{
				this->drawPanelEdge(this->panelEmenyInfos[selectIndex]);
			}
			else
			{
				this->drawPanelEdge(this->panelPlayerInfos[selectIndex - 3]);
			}
		}
		public: virtual void Render() override
		{
			this->renderPlayerInfos();
			this->renderEnemyInfos();
			this->renderBattleInfo();

			this->focusCreaturePanelWhenSelectAsTarget();
		}
		};
	}
}
#endif // !Scene_Battle_H