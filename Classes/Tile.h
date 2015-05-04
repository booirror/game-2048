#ifndef _2048_TILE_H_
#define _2048_TILE_H_

#include "cocos2d.h"

namespace puzzle{

	class Tile : public cocos2d::Node {
	public:
		static Tile* create(int val);
		static Tile* create() { return create(0); }
		virtual void onEnter() override;
		virtual void setPosition(const cocos2d::Vec2& pos) override;
		virtual void setPosition(float x, float y) override { this->setPosition(cocos2d::Vec2(x, y)); }
		virtual const cocos2d::Vec2& getPosition() const override { return position; }
		virtual void setColor(const cocos2d::Color3B& color) override;
		void setValue(int val);
		int getValue() const { return value; }

	private:
		bool init(int val);

		cocos2d::Vec2 position;
		cocos2d::Sprite *image = nullptr;
		cocos2d::Label *valueText = nullptr;
		int value = 0;
		bool marge = false;
	};
}
#endif