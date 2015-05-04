#include "Tile.h"
#include "common.h"
USING_NS_CC;
namespace puzzle{
	static const Color3B tile_color[] = {
		Color3B(220, 210, 200),//0
		Color3B(240, 230, 220),//2
		Color3B(240, 230, 200),//4
		Color3B(240, 170, 120),//8
		Color3B(240, 180, 120),//16
		Color3B(240, 140, 90),//32
		Color3B(240, 120, 90),//64
		Color3B(240, 90, 60),//128
		Color3B(230, 80, 40),//256
		Color3B(240, 60, 40),//512
		Color3B(240, 200, 70),//1024
		Color3B(230, 230, 0),//2048
		Color3B(10, 90, 170)//4096
	};
	static int color_index(int val)
	{
		int exp = 0;
		while (val > 1) {
			val = val / 2;
			exp++;
		}
		return exp;
	}

	Tile* Tile::create(int val)
	{
		auto tile = new Tile();
		if (tile != nullptr && tile->init(val)) {
			tile->autorelease();
			return tile;
		}
		CC_SAFE_DELETE(tile);
		return nullptr;
	}

	void Tile::setPosition(const cocos2d::Vec2& pos)
	{
		position = pos;
		this->image->setPosition(pos);
	}

	void Tile::setColor(const Color3B & color)
	{
		//CCNode::setColor(color);
		image->setColor(color);
	}

	void Tile::setValue(int val)
	{
		value = val;
		if (val == 0) {
			valueText->setString("");
		}
		else {
			char temp[20];
			sprintf(temp, "%d", val);
			valueText->setString(temp);
		}

		if (val > 100)
			valueText->setScaleX(0.85f);
		else if (val > 1000)
			valueText->setScaleX(0.7f);
		else if (val > 10000)
			valueText->setScaleX(0.55f);
		else {
			valueText->setScaleX(1.0f);
		}
		if (val > 4) {
			valueText->setColor(Color3B(255, 255, 255));
		}
		else {
			valueText->setColor(Color3B(60, 60, 60));
		}
		image->setColor(tile_color[color_index(val)]);
	}

	bool Tile::init(int val)
	{
		if (!Node::init()) return false;
		image = Sprite::create("item.png");
		this->addChild(image);
		valueText = Label::createWithTTF(std::string(1, '0' + val), TILE_FONT_NAME, 36);
		valueText->setPosition(Vec2(image->getContentSize().width/2, image->getContentSize().height/2));
		this->image->addChild(valueText);
		this->setValue(val);
		return true;
	}

	void Tile::onEnter()
	{
		Node::onEnter();
		if (value != 0) {
			this->image->setScale(0.7);
			auto scaleToAction = ScaleTo::create(0.1, 1.0);
			this->image->runAction(scaleToAction);
		}
	}
}