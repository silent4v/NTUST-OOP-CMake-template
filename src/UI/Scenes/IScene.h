#ifndef IScene_H
#define IScene_H

#include "../../Entity/Creature.h"
#include "../Panel.h"

namespace FTK
{
	namespace UI
	{
		class IScene
		{
		public: virtual void Awake() = 0;
		public: virtual void Render() = 0;

		protected: void drawCreatureInfo(std::shared_ptr<Creature> creature, UI::UIPanel& panel, const PixelColor& color);
		};
	}
}
#endif // !IScene_H
