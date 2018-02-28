#include "FontPacker.hpp"

namespace Framework
{
	FontPacker::FontPacker(int capacity, int maxCapacity)
	{
		this->capacity = capacity;
		this->maxCapacity = maxCapacity;

		root = new Node(*this, 0, 0, capacity, capacity);

		glGenTextures(1, &textureId);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, capacity, capacity, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	FontPacker::~FontPacker()
	{
		delete root;
	}

	int FontPacker::getCapacity() const
	{
		return capacity;
	}

	GLuint FontPacker::getTextureId() const
	{
		return textureId;
	}

	bool FontPacker::addBitmap(uInt width, uInt height, byte* bitmap, int& left, int& top)
	{
		Node* node = root->findEmptyNode(width, height);

		if (node) {
			left = node->getLeft();
			top = node->getTop();

			glActiveTexture(GL_TEXTURE0);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTextureSubImage2D(textureId, 0, left, top, width, height, GL_RED, GL_UNSIGNED_BYTE, bitmap);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

			return true;
		}
		else if (increaseCapacity()) {
			return addBitmap(width, height, bitmap, left, top);
		}
		else {
			return false;
		}
	}

	bool FontPacker::increaseCapacity()
	{
		if (capacity >= maxCapacity) {
			return false;
		}

		root->increaseCapacity(capacity << 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureId);
		GLubyte* data = new GLubyte[capacity * capacity];
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, capacity << 1, capacity << 1, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
		glTextureSubImage2D(textureId, 0, 0, 0, capacity, capacity, GL_RED, GL_UNSIGNED_BYTE, data);
		delete[] data;

		capacity <<= 1;

		return true;
	}

	FontPacker::Node::Node(const FontPacker& fontPacker, int left, int top, int right, int bottom)
	{
		this->fontPacker = &fontPacker;
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}

	FontPacker::Node::~Node()
	{
		delete childA;
		delete childB;
	}

	int FontPacker::Node::getLeft() const
	{
		return left;
	}

	int FontPacker::Node::getTop() const
	{
		return top;
	}

	FontPacker::Node* FontPacker::Node::findEmptyNode(int width, int height)
	{
		if (childA/* || childB*/) {
			Node* node = childA->findEmptyNode(width, height);
			return node ? node : childB->findEmptyNode(width, height);
		}
		else {
			int dw = right - left - width;
			int dh = bottom - top - height;

			if (filled || dw < (int)(right == fontPacker->getCapacity()) || dh < (int)(bottom == fontPacker->getCapacity())) {
				return nullptr;
			}

			if (dw == 0 && dh == 0) {
				filled = true;
				return this;
			}

			if (dw > dh) {
				childA = new Node(*fontPacker, left, top, left + width, bottom);
				childB = new Node(*fontPacker, left + width, top, right, bottom);
			}
			else {
				childA = new Node(*fontPacker, left, top, right, top + height);
				childB = new Node(*fontPacker, left, top + height, right, bottom);
			}

			return childA->findEmptyNode(width, height);
		}
	}

	void FontPacker::Node::increaseCapacity(int newCapacity)
	{
		if (right == fontPacker->getCapacity()) {
			right = newCapacity;
		}

		if (bottom == fontPacker->getCapacity()) {
			bottom = newCapacity;
		}

		if (childA/* || childB*/) {
			childA->increaseCapacity(newCapacity);
			childB->increaseCapacity(newCapacity);
		}
	}
}
