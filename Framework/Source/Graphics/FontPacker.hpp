#pragma once

#include "System\Globals.hpp"

namespace Framework
{
	class FontPacker
	{
	public:
		FontPacker(int capacity, int maxCapacity);
		~FontPacker();
		int getCapacity() const;
		GLuint getTextureId() const;
		bool addBitmap(uInt width, uInt height, byte bitmap[], int& left, int& top);

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
		GLuint textureId;

		bool increaseCapacity();
	};
}
