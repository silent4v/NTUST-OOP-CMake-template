#ifndef Panel_H
#define Panel_H

#include <iostream>
#include "../Vector2D.hpp"
#include "Pixel.hpp"

namespace FTK
{
	namespace UI
	{
		struct DrawInfo
		{
		public: std::string Content;
		public: PixelColor Color;
		public: Vector2D Offset;
		};

		class UIPanel
		{
		public: Vector2D Position;
		public: int Width;
		public: int Height;
		public: UIPanel() : Position(Vector2D(0, 0)), Width(0), Height(0) {}
		public: UIPanel(const Vector2D& position, int width, int height) :
			Position(position), Width(width), Height(height) {}

		private: void drawString(const std::string& text, int x, int y, const PixelColor& color);
		public: void DrawPanel(const std::string& text, const PixelColor& color = PixelColor(), bool withEdge = true);
		public: void DrawPanel(const std::vector<DrawInfo>& drawInfos, bool withEdge = true);
		public: void DrawPixel(const Vector2D& coord, const Pixel& pixel);
		public: void DrawPixel(int x, int y, char c, const PixelColor& color);
		public: void DrawByLine(const std::string& text, const Vector2D& coord, const PixelColor& color = PixelColor());
		public: void DrawEdge(const PixelColor& color = PixelColor());
		public: void ClearPanel(bool withEdge = true, const PixelColor& color = PixelColor());
		};
	}
}
#endif // !Panel_H