#include "FontPacker.h"

namespace Framework
{
	FontPacker::FontPacker(int capacity, int maxCapacity)
	{
		this->capacity = capacity;
		this->maxCapacity = maxCapacity;

		root = new Node(*this, 0, 0, capacity, capacity);

		texture.createEmpty(capacity, capacity);
	}

	FontPacker::~FontPacker()
	{
		delete root;
	}

	int FontPacker::getCapacity() const
	{
		return capacity;
	}

	void FontPacker::useTexture() const
	{
		texture.use();
	}

	bool FontPacker::addBitmap(uInt width, uInt height, const byte bitmap[], int& left, int& top)
	{
		const Node* node = root->findEmptyNode(width, height);

		if (node) {
			left = node->getLeft();
			top = node->getTop();

			texture.update(left, top, width, height, bitmap);

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

		root->increaseCapacity(capacity << 1); // uses this font packer's capacity internally

		capacity <<= 1;

		texture.resize(capacity, capacity);

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

	const FontPacker::Node* FontPacker::Node::findEmptyNode(int width, int height)
	{
		if (childA/* || childB*/) {
			const Node* node = childA->findEmptyNode(width, height);
			return node ? node : childB->findEmptyNode(width, height);
		}
		else {
			int dw = right - left - width;
			int dh = bottom - top - height;

			if (filled || dw < (int)(right == fontPacker->capacity) || dh < (int)(bottom == fontPacker->capacity)) {
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
		if (right == fontPacker->capacity) {
			right = newCapacity;
		}

		if (bottom == fontPacker->capacity) {
			bottom = newCapacity;
		}

		if (childA/* || childB*/) {
			childA->increaseCapacity(newCapacity);
			childB->increaseCapacity(newCapacity);
		}
	}
}
