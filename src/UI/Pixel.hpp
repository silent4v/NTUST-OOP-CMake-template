#ifndef Pixel_H

#define Pixel_H

#include <iostream>
#include <vector>
#include <Windows.h>

#include "../Define/Color.h"
#include "../Util/Singleton.hpp"
#include "../Vector2D.hpp"

namespace FTK
{
	namespace UI
	{
		struct PixelColor
		{
		public: int ForeGroundColor, BackGroundColor;
		public: PixelColor(const PixelColor& other) :
			ForeGroundColor(other.ForeGroundColor), BackGroundColor(other.BackGroundColor) {}
		public: PixelColor(int foreGroundColor = 0x0f, int backGroundColor = 0x00) :
			ForeGroundColor(foreGroundColor), BackGroundColor(backGroundColor) {}
		public: PixelColor(EmColor foreGroundColor, EmColor backGroundColor) :
			ForeGroundColor(Util::ForeGroundColor(foreGroundColor)), BackGroundColor(Util::BackGroundColor(backGroundColor)) {}
		public: bool operator==(const PixelColor& other) const
		{
			return ForeGroundColor == other.ForeGroundColor && BackGroundColor == other.BackGroundColor;
		}
		public: bool operator!=(const PixelColor& other) const
		{
			return !(*this == other);
		}
		};

		struct Pixel
		{
		public: wchar_t Character;
		public: PixelColor Color;
		public: Pixel(wchar_t character, PixelColor color) :
			Character(character), Color(color) {}
		public: Pixel(wchar_t character = ' ', int foreGroundColor = 0x0f, int backGroundColor = 0x00) :
			Character(character), Color(foreGroundColor, backGroundColor) {}
		public: bool operator==(const Pixel& other) const
		{
			return Character == other.Character && Color == other.Color;
		}
		};
	}
}
#endif // !UIManager