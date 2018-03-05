#pragma once

#include "OpenGL\Texture2D.h"

namespace Framework
{
	class FontPacker
	{
	public:
		FontPacker(int capacity, int maxCapacity);
		~FontPacker();
		int getCapacity() const;
		void useTexture() const;
		bool addBitmap(uInt width, uInt height, const byte bitmap[], int& x, int& y);

	private:
		class Node
		{
		public:
			Node(const FontPacker& fontPacker, int left, int top, int right, int bottom);
			~Node();
			int getLeft() const;
			int getTop() const;
			const Node* findEmptyNode(int width, int height);
			void increaseCapacity(int newCapacity);

		private:
			const FontPacker* fontPacker;
			int left, top, right, bottom;
			bool filled = false;
			Node* childA = nullptr;
			Node* childB = nullptr;
		};

		int capacity;
		int maxCapacity;
		Node* root = nullptr;
		Texture2D texture;

		bool increaseCapacity();
	};
}
