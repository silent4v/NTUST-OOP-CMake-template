#ifndef Color_H
#define Color_H

enum class EmColor
{
	Black = 0x00,
	Blue = 0x01,
	Green = 0x02,
	Cyan = 0x03,
	Red = 0x04,
	Magenta = 0x05,
	Brown = 0x06,
	LightGray = 0x07,
	DarkGray = 0x08,
	LightBlue = 0x09,
	LightGreen = 0x0A,
	LightCyan = 0x0B,
	LightRed = 0x0C,
	LightMagenta = 0x0D,
	Yellow = 0x0E,
	White = 0x0F
};

namespace Util
{
	inline static int ForeGroundColor(const EmColor& color)
	{
		return static_cast<int>(color);
	}

	inline static int BackGroundColor(const EmColor& color)
	{
		return static_cast<int>(color) << 4;
	}
}
#endif // !Color_H