#include "pch.h"
#include "CppUnitTest.h"

#include "../src/Battle/BattleManager.h"
#include "../src/Item/Equipments/Equipments.h"
#include "../src/Skill/Skills.h"
#include "../src/Util/LoDash/LoDash.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FTK;

namespace UnitTest
{
	TEST_CLASS(TEST_BattleManager)
	{
	private: std::vector<std::shared_ptr<ActiveSkill>> getActiveSkill(std::shared_ptr<Creature> creature)
	{
		return LoDash::Map(
			LoDash::Where(
				creature->Skills(),
				[](const std::shared_ptr<ISkill>& skill)
				{
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
	}
	private: std::shared_ptr<ActiveSkill> selectActiveSkill(std::shared_ptr<Creature> creature, EmSkill skill)
	{
		auto activeSkills = this->getActiveSkill(creature);

		for (auto& activeSkill : activeSkills)
		{
			if (activeSkill->SkillType == skill)
			{
				return activeSkill;
			}
		}

		return nullptr;
	}
	private: std::shared_ptr<ISkill> selectSkill(std::shared_ptr<Creature> creature, EmSkill skill)
	{
		for (auto& s : creature->Skills())
		{
			if (s->SkillType == skill)
			{
				return s;
			}
		}

		return nullptr;
	}

	public:
		TEST_METHOD(Test_NormalAttack)
		{
			std::vector<std::shared_ptr<Player>> battlePlayers{
				std::make_shared<Player>(
					"Player2",
					UI::Pixel{
						L'2',
						UI::PixelColor(Util::ForeGroundColor(EmColor::White))
					},
					Entity::EmType::Player,
					Vector2D(50, 11),
					Attribute({ 12, 6, 4, 0, 0, 0, 80, 100, }),
					nullptr
				)
			};
			for (auto& player : battlePlayers)
			{
				player->SetSkills({
					std::make_shared<NormalAttack>(player, 1),
					std::make_shared<FTK::Escape>(player)
					}
				);
			}
			std::vector<std::shared_ptr<Creature>> battleEnemys{
				std::make_shared<Creature>(
					"Enemy1",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(50, 10),
					Attribute({ 200, 0, 10, 0, 0, 0, 100, 50, })
				),
				std::make_shared<Creature>(
					"Enemy2",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(50, 10),
					Attribute({ 20, 0, 10, 0, 0, 0, 100, 50, })
				),
				std::make_shared<Creature>(
					"Enemy3",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(50, 9),
					Attribute({ 20, 0, 10, 0, 0, 0, 100, 50, })
				),
			};
			for (auto& e : battleEnemys)
			{
				e->SetSkills({ std::make_shared<NormalAttack>(e, 1) });
			}

			auto battleEntities = std::make_shared<BattleEntities>(
				battlePlayers,
				battleEnemys
			);
			auto battleManager = std::make_shared<BattleManager>(battleEntities);
			int playerSelectActionCounter = 0;
			battleManager->Event_WaitingCreatureSelectAction.Register([&](std::shared_ptr<Creature> creature)
				{
					if (creature->Type() == Entity::EmType::Player)
						playerSelectActionCounter++;
				}
			);
			Assert::IsTrue(
				Battle::ProcessController::EmState::BattleStart == battleManager->processController->State()
			);

			battleManager->Process();
			Assert::IsTrue(
				Battle::ProcessController::EmState::CreatureTurn == battleManager->processController->State()
			);
			Assert::AreEqual(1, playerSelectActionCounter);
			Assert::IsTrue(battleManager->processController->CurrentCreature() == battlePlayers[0]);

			auto selectSkill = this->selectActiveSkill(battlePlayers[0], EmSkill::NormalAttack);
			Assert::IsTrue(selectSkill != nullptr);
			battleManager->Battle(
				std::static_pointer_cast<ActiveBattleInfo>(
					selectSkill->BattleInfo(
						{ battleEnemys[0] },
						battleEntities,
						1
					)
				)
			);
			Assert::IsTrue(
				Battle::ProcessController::EmState::CreatureTurnEnd == battleManager->processController->State()
			);

			Assert::AreEqual(200 - 4, battleEnemys[0]->CurrentAttribute().HP);
			Assert::AreEqual(5, battlePlayers[0]->CurrentAttribute().Focus);
		}
		TEST_METHOD(Test_HammerSplash)
		{
			std::vector<std::shared_ptr<Player>> battlePlayers{
				std::make_shared<Player>(
					"Player2",
					UI::Pixel{
						L'2',
						UI::PixelColor(Util::ForeGroundColor(EmColor::White))
					},
					Entity::EmType::Player,
					Vector2D(50, 11),
					Attribute({ 12, 6, 4, 0, 0, 0, 80, 100, }),
					nullptr
				)
			};
			for (auto& player : battlePlayers)
			{
				player->SetSkills({
					std::make_shared<NormalAttack>(player, 1),
					std::make_shared<FTK::Escape>(player)
					}
				);
			}
			battlePlayers[0]->Wear(
				std::make_shared<item::Hammer>()
			);

			std::vector<std::shared_ptr<Creature>> battleEnemys{
				std::make_shared<Creature>(
					"Enemy1",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(50, 10),
					Attribute({ 200, 0, 10, 0, 0, 0, 100, 50, })
				),
				std::make_shared<Creature>(
					"Enemy2",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(50, 10),
					Attribute({ 20, 0, 10, 0, 0, 0, 100, 50, })
				),
				std::make_shared<Creature>(
					"Enemy3",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(50, 9),
					Attribute({ 20, 0, 10, 0, 0, 0, 100, 50, })
				),
			};
			for (auto& e : battleEnemys)
			{
				e->SetSkills({ std::make_shared<NormalAttack>(e, 1) });
			}

			auto battleEntities = std::make_shared<BattleEntities>(
				battlePlayers,
				battleEnemys
			);
			auto battleManager = std::make_shared<BattleManager>(battleEntities);
			int playerSelectActionCounter = 0;
			battleManager->Event_WaitingCreatureSelectAction.Register([&](std::shared_ptr<Creature> creature)
				{
					if (creature->Type() == Entity::EmType::Player)
						playerSelectActionCounter++;
				}
			);

			Assert::IsTrue(
				Battle::ProcessController::EmState::BattleStart == battleManager->processController->State()
			);

			battleManager->Process();
			Assert::IsTrue(
				Battle::ProcessController::EmState::CreatureTurn == battleManager->processController->State()
			);
			Assert::AreEqual(1, playerSelectActionCounter);
			Assert::IsTrue(battleManager->processController->CurrentCreature() == battlePlayers[0]);

			auto selectSkill = this->selectActiveSkill(battlePlayers[0], EmSkill::NormalAttack);
			Assert::IsTrue(selectSkill != nullptr);
			battleManager->Battle(
				std::static_pointer_cast<ActiveBattleInfo>(
					selectSkill->BattleInfo(
						{ battleEnemys[0] },
						battleEntities,
						4
					)
				)
			);
			Assert::IsTrue(
				Battle::ProcessController::EmState::CreatureTurnEnd == battleManager->processController->State()
			);

			Assert::AreEqual(200 - 19, battleEnemys[0]->CurrentAttribute().HP);
			Assert::AreEqual(20 - 9, battleEnemys[1]->CurrentAttribute().HP);
			Assert::AreEqual(20 - 9, battleEnemys[2]->CurrentAttribute().HP);
			Assert::AreEqual(2, battlePlayers[0]->CurrentAttribute().Focus);
		}
		TEST_METHOD(Test_Heal)
		{
			std::vector<std::shared_ptr<Player>> battlePlayers{
				std::make_shared<Player>(
					"Player2",
					UI::Pixel{
						L'2',
						UI::PixelColor(Util::ForeGroundColor(EmColor::White))
					},
					Entity::EmType::Player,
					Vector2D(50, 11),
					Attribute({ 100, 6, 4, 0, 0, 0, 80, 51, }),
					nullptr
				)
			};
			for (auto& player : battlePlayers)
			{
				player->SetSkills({
					std::make_shared<NormalAttack>(player, 1),
					std::make_shared<FTK::Escape>(player)
					}
				);
			}
			battlePlayers[0]->Wear(
				std::make_shared<item::MagicWand>()
			);

			std::vector<std::shared_ptr<Creature>> battleEnemys{
				std::make_shared<Creature>(
					"Enemy1",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(50, 10),
					Attribute({ 200, 0, 10, 0, 0, 0, 100, 50, })
				),
			};
			for (auto& e : battleEnemys)
			{
				e->SetSkills({ std::make_shared<NormalAttack>(e, 1) });
			}

			auto battleEntities = std::make_shared<BattleEntities>(
				battlePlayers,
				battleEnemys);
			battlePlayers[0]->Hurt(50);
			auto battleManager = std::make_shared<BattleManager>(battleEntities);
			Assert::AreEqual(50, battlePlayers[0]->CurrentAttribute().HP);
			battleManager->Process();
			Assert::IsTrue(battleManager->processController->CurrentCreature() == battlePlayers[0]);
			auto selectSkill = this->selectActiveSkill(battlePlayers[0], EmSkill::Heal);
			Assert::IsTrue(selectSkill != nullptr);
			battleManager->Battle(
				std::static_pointer_cast<ActiveBattleInfo>(
					selectSkill->BattleInfo(
						{ battlePlayers[0] },
						battleEntities,
						2
					)
				)
			);
			Assert::IsTrue(
				Battle::ProcessController::EmState::CreatureTurnEnd == battleManager->processController->State()
			);

			Assert::AreEqual(65, battlePlayers[0]->CurrentAttribute().HP);
			Assert::AreEqual(4, battlePlayers[0]->CurrentAttribute().Focus);
		}
		TEST_METHOD(Test_Heal_Overflow)
		{
			std::vector<std::shared_ptr<Player>> battlePlayers{
				std::make_shared<Player>(
					"Player2",
					UI::Pixel{
						L'2',
						UI::PixelColor(Util::ForeGroundColor(EmColor::White))
					},
					Entity::EmType::Player,
					Vector2D(50, 11),
					Attribute({ 100, 6, 4, 0, 0, 0, 80, 51, }),
					nullptr
				)
			};
			for (auto& player : battlePlayers)
			{
				player->SetSkills({
					std::make_shared<NormalAttack>(player, 1),
					std::make_shared<FTK::Escape>(player)
					}
				);
			}
			battlePlayers[0]->Wear(
				std::make_shared<item::MagicWand>()
			);

			std::vector<std::shared_ptr<Creature>> battleEnemys{
				std::make_shared<Creature>(
					"Enemy1",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(50, 10),
					Attribute({ 200, 0, 10, 0, 0, 0, 100, 50, })
				),
			};
			for (auto& e : battleEnemys)
			{
				e->SetSkills({ std::make_shared<NormalAttack>(e, 1) });
			}

			auto battleEntities = std::make_shared<BattleEntities>(
				battlePlayers,
				battleEnemys);
			battlePlayers[0]->Hurt(5);
			auto battleManager = std::make_shared<BattleManager>(battleEntities);
			Assert::AreEqual(95, battlePlayers[0]->CurrentAttribute().HP);
			battleManager->Process();
			Assert::IsTrue(battleManager->processController->CurrentCreature() == battlePlayers[0]);
			auto selectSkill = this->selectActiveSkill(battlePlayers[0], EmSkill::Heal);
			Assert::IsTrue(selectSkill != nullptr);
			battleManager->Battle(
				std::static_pointer_cast<ActiveBattleInfo>(
					selectSkill->BattleInfo(
						{ battlePlayers[0] },
						battleEntities,
						2
					)
				)
			);
			Assert::IsTrue(
				Battle::ProcessController::EmState::CreatureTurnEnd == battleManager->processController->State()
			);

			Assert::AreEqual(100, battlePlayers[0]->CurrentAttribute().HP);
			Assert::AreEqual(4, battlePlayers[0]->CurrentAttribute().Focus);
		}

		TEST_METHOD(Test_Provoke)
		{
			std::vector<std::shared_ptr<Player>> battlePlayers{
				std::make_shared<Player>(
					"Player2",
					UI::Pixel{
						L'2',
						UI::PixelColor(Util::ForeGroundColor(EmColor::White))
					},
					Entity::EmType::Player,
					Vector2D(50, 11),
					Attribute({ 100, 6, 4, 0, 0, 0, 80, 51, }),
					nullptr
				)
			};
			for (auto& player : battlePlayers)
			{
				player->SetSkills({
					std::make_shared<NormalAttack>(player, 1),
					std::make_shared<FTK::Escape>(player)
					}
				);
			}
			battlePlayers[0]->Wear(
				std::make_shared<item::Hammer>()
			);

			std::vector<std::shared_ptr<Creature>> battleEnemys{
				std::make_shared<Creature>(
					"Enemy1",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(50, 10),
					Attribute({ 200, 0, 10, 0, 0, 0, 100, 50, })
				),
			};
			for (auto& e : battleEnemys)
			{
				e->SetSkills({ std::make_shared<NormalAttack>(e, 1) });
			}

			auto battleEntities = std::make_shared<BattleEntities>(
				battlePlayers,
				battleEnemys);
			auto battleManager = std::make_shared<BattleManager>(battleEntities);
			battleManager->Process();
			auto selectSkill = this->selectActiveSkill(battlePlayers[0], EmSkill::Provoke);
			Assert::IsTrue(selectSkill != nullptr);
			battleManager->Battle(
				std::static_pointer_cast<ActiveBattleInfo>(
					selectSkill->BattleInfo(
						{ battleEnemys[0] },
						battleEntities,
						1
					)
				)
			);
			Assert::IsTrue(
				Battle::ProcessController::EmState::CreatureTurnEnd == battleManager->processController->State()
			);

			Assert::IsTrue(
				LoDash::Any(battleEnemys[0]->Buffs(),
					[](const std::shared_ptr<IBuff>& buff)
					{
						return buff->Type == EmBuff::Angry;
					}
				)
			);
		}

		TEST_METHOD(Test_Fority)
		{
			std::vector<std::shared_ptr<Player>> battlePlayers{
				std::make_shared<Player>(
					"Player2",
					UI::Pixel{
						L'2',
						UI::PixelColor(Util::ForeGroundColor(EmColor::White))
					},
					Entity::EmType::Player,
					Vector2D(50, 11),
					Attribute({ 12, 6, 100, 0, 0, 0, 80, 100, }),
					nullptr
				)
			};
			for (auto& player : battlePlayers)
			{
				player->SetSkills({
					std::make_shared<NormalAttack>(player, 1),
					std::make_shared<FTK::Escape>(player)
					}
				);
			}
			std::vector<std::shared_ptr<Creature>> battleEnemys{
				std::make_shared<Creature>(
					"Enemy1",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(50, 10),
					Attribute({ 200, 0, 10, 0, 0, 0, 100, 50, })
				),
			};
			for (auto& e : battleEnemys)
			{
				e->SetSkills({ std::make_shared<NormalAttack>(e, 1) });
			}
			battleEnemys[0]->Wear(
				std::make_shared<item::PlateArmor>()
			);

			auto battleEntities = std::make_shared<BattleEntities>(
				battlePlayers,
				battleEnemys
			);
			auto battleManager = std::make_shared<BattleManager>(battleEntities);
			battleManager->Process();
			{
				Assert::IsTrue(
					Battle::ProcessController::EmState::CreatureTurn == battleManager->processController->State()
				);

				Assert::IsTrue(battleManager->processController->CurrentCreature() == battlePlayers[0]);
				auto selectSkill = this->selectActiveSkill(battlePlayers[0], EmSkill::NormalAttack);
				Assert::IsTrue(selectSkill != nullptr);
				battleManager->Battle(
					std::static_pointer_cast<ActiveBattleInfo>(
						selectSkill->BattleInfo(
							{ battleEnemys[0] },
							battleEntities,
							1
						)
					)
				);
				Assert::IsTrue(
					Battle::ProcessController::EmState::CreatureTurnEnd == battleManager->processController->State()
				);

				Assert::AreEqual(139, battleEnemys[0]->CurrentAttribute().HP); // int(200 - (100 - 20) * .9)
				auto fortify = this->selectSkill(battleEnemys[0], EmSkill::Fortify);
				Assert::IsTrue(fortify != nullptr);
				Assert::AreEqual(5, fortify->CurrentCoolDowning());
			}

			battleManager->Process();
			{
				Assert::IsTrue(
					Battle::ProcessController::EmState::CreatureTurn == battleManager->processController->State()
				);

				Assert::IsTrue(battleManager->processController->CurrentCreature() == battlePlayers[0]);
				auto selectSkill = this->selectActiveSkill(battlePlayers[0], EmSkill::NormalAttack);
				Assert::IsTrue(selectSkill != nullptr);
				battleManager->Battle(
					std::static_pointer_cast<ActiveBattleInfo>(
						selectSkill->BattleInfo(
							{ battleEnemys[0] },
							battleEntities,
							1
						)
					)
				);
				Assert::IsTrue(
					Battle::ProcessController::EmState::CreatureTurnEnd == battleManager->processController->State()
				);
				Assert::AreEqual(68, battleEnemys[0]->CurrentAttribute().HP); // 128-80
				auto fortify = this->selectSkill(battleEnemys[0], EmSkill::Fortify);
				Assert::IsTrue(fortify != nullptr);
				Assert::AreEqual(5, fortify->CurrentCoolDowning());
			}
		}

		TEST_METHOD(Test_Destory)
		{
			std::vector<std::shared_ptr<Player>> battlePlayers{
				std::make_shared<Player>(
					"Player2",
					UI::Pixel{
						L'2',
						UI::PixelColor(Util::ForeGroundColor(EmColor::White))
					},
					Entity::EmType::Player,
					Vector2D(50, 11),
					Attribute({ 12, 6, 100, 0, 0, 0, 80, 100, }),
					nullptr
				)
			};
			for (auto& player : battlePlayers)
			{
				player->SetSkills({
					std::make_shared<NormalAttack>(player, 1),
					std::make_shared<FTK::Escape>(player),
					std::make_shared<FTK::Destory>(player)
					}
				);
			}
			std::vector<std::shared_ptr<Creature>> battleEnemys{
				std::make_shared<Creature>(
					"Enemy1",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(50, 10),
					Attribute({ 200, 0, 10, 0, 0, 0, 100, 50, })
				),
			};
			for (auto& e : battleEnemys)
			{
				e->SetSkills({ std::make_shared<NormalAttack>(e, 1) });
			}
			battleEnemys[0]->Wear(
				std::make_shared<item::PlateArmor>()
			);

			auto battleEntities = std::make_shared<BattleEntities>(
				battlePlayers,
				battleEnemys
			);
			auto battleManager = std::make_shared<BattleManager>(battleEntities);
			battleManager->Process();
			{
				Assert::IsTrue(
					Battle::ProcessController::EmState::CreatureTurn == battleManager->processController->State()
				);

				Assert::IsTrue(battleManager->processController->CurrentCreature() == battlePlayers[0]);
				auto selectSkill = this->selectActiveSkill(battlePlayers[0], EmSkill::NormalAttack);
				Assert::IsTrue(selectSkill != nullptr);
				battleManager->Battle(
					std::static_pointer_cast<ActiveBattleInfo>(
						selectSkill->BattleInfo(
							{ battleEnemys[0] },
							battleEntities,
							1
						)
					)
				);
				Assert::IsTrue(
					Battle::ProcessController::EmState::CreatureTurnEnd == battleManager->processController->State()
				);

				Assert::AreEqual(139, battleEnemys[0]->CurrentAttribute().HP); // int(200 - (100 - 20) * .9)
				auto fortify = this->selectSkill(battleEnemys[0], EmSkill::Fortify);
				Assert::IsTrue(fortify == nullptr);
			}

			battleManager->Process();
			{
				Assert::IsTrue(
					Battle::ProcessController::EmState::CreatureTurn == battleManager->processController->State()
				);

				Assert::IsTrue(battleManager->processController->CurrentCreature() == battlePlayers[0]);
				auto selectSkill = this->selectActiveSkill(battlePlayers[0], EmSkill::NormalAttack);
				Assert::IsTrue(selectSkill != nullptr);
				battleManager->Battle(
					std::static_pointer_cast<ActiveBattleInfo>(
						selectSkill->BattleInfo(
							{ battleEnemys[0] },
							battleEntities,
							1
						)
					)
				);
				Assert::IsTrue(
					Battle::ProcessController::EmState::CreatureTurnEnd == battleManager->processController->State()
				);
				Assert::AreEqual(39, battleEnemys[0]->CurrentAttribute().HP); // 128-100
			}
		}

		TEST_METHOD(Test_LaurelWreath)
		{
			std::vector<std::shared_ptr<Player>> battlePlayers{
				std::make_shared<Player>(
					"Player2",
					UI::Pixel{
						L'2',
						UI::PixelColor(Util::ForeGroundColor(EmColor::White))
					},
					Entity::EmType::Player,
					Vector2D(50, 11),
					Attribute({ 24, 6, 100, 24, 0, 0, 80, 100, }),
					nullptr
				)
			};
			for (auto& player : battlePlayers)
			{
				player->SetSkills({
					std::make_shared<NormalAttack>(player, 1),
					std::make_shared<FTK::Escape>(player),
					std::make_shared<FTK::Destory>(player)
					}
				);
			}
			std::vector<std::shared_ptr<Creature>> battleEnemys{
				std::make_shared<Creature>(
					"Enemy1",
					UI::Pixel{
						L'E',
						UI::PixelColor(Util::ForeGroundColor(EmColor::Red))
					},
					Entity::EmType::Enemy,
					Vector2D(50, 10),
					Attribute({ 200, 0, 6, 10, 0, 0, 100, 50, })
				),
			};
			for (auto& e : battleEnemys)
			{
				e->SetSkills({ std::make_shared<NormalAttack>(e, 1) });
			}
			battleEnemys[0]->Wear(
				std::make_shared<item::LaurelWreath>()
			);

			auto battleEntities = std::make_shared<BattleEntities>(
				battlePlayers,
				battleEnemys
			);
			auto battleManager = std::make_shared<BattleManager>(battleEntities);
			battleManager->Process();
			{
				Assert::IsTrue(
					Battle::ProcessController::EmState::CreatureTurn == battleManager->processController->State()
				);

				Assert::IsTrue(battleManager->processController->CurrentCreature() == battlePlayers[0]);
				auto selectSkill = this->selectActiveSkill(battlePlayers[0], EmSkill::NormalAttack);
				Assert::IsTrue(selectSkill != nullptr);
				battleManager->Battle(
					std::static_pointer_cast<ActiveBattleInfo>(
						selectSkill->BattleInfo(
							{ battleEnemys[0] },
							battleEntities,
							1
						)
					)
				);
				Assert::IsTrue(
					Battle::ProcessController::EmState::CreatureTurnEnd == battleManager->processController->State()
				);

				// defense = 10*1.1
				// reduceRate = 11/(11+50) = 0.1803
				// 100 * (1 - 0.1803) ~= 81
				// 200 - 81 = 119
				Assert::AreEqual(119, battleEnemys[0]->CurrentAttribute().HP); 
			}
		}
	};
}
