#pragma once

#include "OpenGL\Texture2D.h"

namespace GLEngine
{
	class FontPacker
	{
	public:
		FontPacker(int capacity, int maxCapacity);
		~FontPacker();
		int getCapacity() const;
		bool addBitmap(int width, int height, const Byte bitmap[], int* x, int* y);
		void useTexture() const;

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

		bool increaseCapacity();

		int capacity;
		int maxCapacity;
		Node* root;
		Texture2D texture;
	};
}
