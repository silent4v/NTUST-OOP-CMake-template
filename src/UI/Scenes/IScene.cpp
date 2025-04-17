#include "IScene.h"

namespace FTK::UI
{
	void IScene::drawCreatureInfo(std::shared_ptr<Creature> creature, UI::UIPanel& panel, const PixelColor& color)
	{
		panel.DrawByLine(
			"Name: " + creature->Name(),
			Vector2D(1, 1),
			color
		);
		panel.DrawByLine(
			"HP: " + std::to_string(creature->CurrentAttribute().HP) + "/" + std::to_string(creature->ModifyAttribute().HP) + ",\t" +
			"Focus: " + std::to_string(creature->CurrentAttribute().Focus) + "/" + std::to_string(creature->ModifyAttribute().Focus),
			Vector2D(1, 2),
			color
		);
		panel.DrawByLine(
			"Physical ATK: " + std::to_string(creature->CurrentAttribute().PhysicalAttack) + ",\t" +
			"Physical DEF: " + std::to_string(creature->CurrentAttribute().PhysicalDefense),
			Vector2D(1, 3),
			color
		);
		panel.DrawByLine(
			"Magical ATK: " + std::to_string(creature->CurrentAttribute().MagicalAttack) + ",\t" +
			"Magical DEF: " + std::to_string(creature->CurrentAttribute().MagicalDefense),
			Vector2D(1, 4),
			color
		);
		panel.DrawByLine(
			"Speed: " + std::to_string(creature->CurrentAttribute().Speed) + ",\t" +
			"HitRate: " + std::to_string(creature->CurrentAttribute().HitRate),
			Vector2D(1, 5),
			color
		);
		panel.DrawByLine(
			"Weapon: " + (creature->Weapon() == nullptr ? "" : creature->Weapon()->Name),
			Vector2D(1, 6),
			color
		);
		panel.DrawByLine(
			"Armor: " + (creature->Armor() == nullptr ? "" : creature->Armor()->Name),
			Vector2D(1, 7),
			color
		);
		panel.DrawByLine(
			"Accessory: " + (creature->Accessory() == nullptr ? "" : creature->Accessory()->Name),
			Vector2D(1, 8),
			color
		);
		std::string buffStr = "Buff:";
		for(const auto& buff : creature->Buffs())
		{
			buffStr += buff->Notation;
		}
		panel.DrawByLine
			(
			buffStr,
			Vector2D(1, 9),
			color
		);
	}
}