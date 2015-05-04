#include <functional>
#include "GameScene.h"
#include "common.h"
#include "Tile.h"
#include "SimulateGame.h"
USING_NS_CC;
using namespace std::placeholders;
#define TILE_NUM 16
#define TAG_RESULT_NODE 80
#define TAG_ADD  81
#define target_val 2048
static const float min_valid_move = 100.0f;

enum Sprite_ZOrder{
	ZORDER_BG_LAYER,
	ZORDER_TILE_LAYER,
	ZORDER_RESULT_LAYER
};

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bgimg = Sprite::create("background.png");
	bgimg->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	this->addChild(bgimg, ZORDER_BG_LAYER);

	auto name_label = Label::createWithTTF(TTFConfig(GAME_FONT_NAME, 32), "2048");
	name_label->setColor(Color3B(0x5B, 0x5B, 0x5B));
	name_label->setPosition(Vec2(visibleSize.width*0.22 + origin.x, visibleSize.height*0.86 + origin.y));
	this->addChild(name_label, ZORDER_BG_LAYER);


	//Score, best
	auto score = Sprite::create("item.png");
	auto scalex = 1.0f;
	auto scaley = 0.8f;
	Color3B valcolor = Color3B(0xA5, 0xA5, 0xA5);
	auto spos = Vec2(visibleSize.width*0.6 + origin.x, visibleSize.height*0.90 + origin.y);
	score->setPosition(spos);
	score->setScaleX(scalex);
	score->setScaleY(scaley);
	this->addChild(score, ZORDER_BG_LAYER);
	auto score_label = Label::createWithTTF(TTFConfig(SCORE_FONT_NAME, 24), "SCORE");
	score_label->setColor(Color3B(240, 180, 120));
	score_label->setPosition(score->getContentSize().width*scalex/2, score->getContentSize().height*scaley*0.9);
	score->addChild(score_label, ZORDER_BG_LAYER);
	scoreValLabel = Label::createWithTTF(TTFConfig(SCORE_FONT_NAME, 24), "0");
	scoreValLabel->setColor(valcolor);
	scoreValLabel->setPosition(score_label->getPositionX(), score_label->getPositionY() - score_label->getContentSize().height - 10);
	score->addChild(scoreValLabel, ZORDER_BG_LAYER);

	auto best_bg = Sprite::create("item.png");
	best_bg->setPosition(spos.x + visibleSize.width*0.25, spos.y);
	best_bg->setScaleX(scalex);
	best_bg->setScaleY(scaley);
	this->addChild(best_bg, ZORDER_BG_LAYER);
	auto best_label = Label::createWithTTF(TTFConfig(SCORE_FONT_NAME, 24), "BEST");
	best_label->setColor(Color3B(240, 180, 120));
	best_label->setPosition(Vec2(best_bg->getContentSize().width*scalex / 2, best_bg->getContentSize().height*scaley*0.9));
	best_bg->addChild(best_label, ZORDER_BG_LAYER);
	bestValLabel = Label::createWithTTF(TTFConfig(SCORE_FONT_NAME, 24), "0");
	bestValLabel->setColor(valcolor);
	bestValLabel->setPosition(best_label->getPositionX(), score_label->getPositionY() - best_label->getContentSize().height - 10);
	best_bg->addChild(bestValLabel, ZORDER_BG_LAYER);

	//new game
	auto menu = Menu::create();
	menu->setPosition(Vec2(0, 0));
	this->addChild(menu, ZORDER_BG_LAYER);
	auto btsprite = Sprite::create("item.png");
	btsprite->setScaleY(0.5);
	btsprite->setColor(Color3B(0x8B, 0x45, 0x13));
	btsprite->setContentSize(Size(btsprite->getContentSize().width, btsprite->getContentSize().height*btsprite->getScaleY()));
	auto newbtn = MenuItemSprite::create(btsprite, btsprite, std::bind(&GameScene::onNewGame, this, _1));
	newbtn->setPosition(Vec2(visibleSize.width*0.75, visibleSize.height*0.80));
	auto btnlabel = Label::createWithTTF(TTFConfig(SCORE_FONT_NAME, 16), "New Game");
	btnlabel->setPosition(newbtn->getContentSize().width / 2, newbtn->getContentSize().height/2);
	newbtn->addChild(btnlabel);
	menu->addChild(newbtn);

	//AI btn
	auto aibt = Sprite::create("item.png");
	aibt->setScaleY(0.5);
	aibt->setColor(Color3B(0x8B, 0x45, 0x13));
	aibt->setContentSize(Size(aibt->getContentSize().width, aibt->getContentSize().height*aibt->getScaleY()));
	auto ai = MenuItemSprite::create(aibt, aibt, std::bind(&GameScene::onAI, this, _1));
	ai->setPosition(Vec2(visibleSize.width*0.45, visibleSize.height*0.80));
	auto ailabel = Label::createWithTTF(TTFConfig(SCORE_FONT_NAME, 16), "AI");
	ailabel->setPosition(ai->getContentSize().width / 2, ai->getContentSize().height / 2);
	ai->addChild(ailabel);
	menu->addChild(ai);

	auto testsp = Sprite::create("item.png");
	testsp->setScaleY(0.5);
	testsp->setColor(Color3B(0x8B, 0x45, 0x13));
	testsp->setContentSize(Size(testsp->getContentSize().width, testsp->getContentSize().height*testsp->getScaleY()));
	auto testItem = MenuItemSprite::create(testsp, testsp, std::bind(&GameScene::onTest, this, _1));
	testItem->setPosition(Vec2(visibleSize.width*0.15, visibleSize.height*0.80));
	auto testLabel = Label::createWithTTF(TTFConfig(SCORE_FONT_NAME, 16), "Test");
	testLabel->setPosition(testItem->getContentSize().width / 2, testItem->getContentSize().height / 2);
	testItem->addChild(testLabel);
	menu->addChild(testItem);

	touchRect = Rect(visibleSize.width*0.07, visibleSize.height*0.1,
		visibleSize.width*0.86, visibleSize.height*0.56);

	auto eventDispatchar = Director::getInstance()->getEventDispatcher();

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = std::bind(&GameScene::singleTouchBegan, this, _1, _2);
	touchListener->onTouchMoved = std::bind(&GameScene::singleTouchMoved, this, _1, _2);
	touchListener->onTouchEnded = std::bind(&GameScene::singleTouchEnded, this, _1, _2);
	touchListener->onTouchCancelled = std::bind(&GameScene::singleTouchCancelled, this, _1, _2);
	eventDispatchar->addEventListenerWithSceneGraphPriority(touchListener, this);

	//add keyboard listener
	auto keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = std::bind(&GameScene::onKeyDown, this, _1, _2);
	keyListener->onKeyReleased = std::bind(&GameScene::onKeyUp, this, _1, _2);
	eventDispatchar->addEventListenerWithSceneGraphPriority(keyListener, this);

	for (int i = 0; i < TILE_NUM; i++) {
		auto tile = puzzle::Tile::create(0);
		//tile->setColor(Color3B(0xe5, 0xe5, 0xe5));
		tile->setPosition(caculateTilePos(i/4, i%4));
		this->addChild(tile, ZORDER_BG_LAYER);
	}
	resetGame();
	auto t1 = genRandomTile();
	auto t2 = genRandomTile();
	this->addChild(t1, ZORDER_TILE_LAYER);
	this->addChild(t2, ZORDER_TILE_LAYER);
	state = GameState::running;
	log("init end");
    return true;
}

void GameScene::mergeTiles(int targetRow, int col, puzzle::Tile* prevTile, puzzle::Tile* currTile)
{
	state = GameState::moving;
	this->moveCount++;
	auto mtAction = MoveTo::create(0.1f, caculateTilePos(targetRow, col));
	auto func = CallFunc::create([this, prevTile, currTile, targetRow, col]()->void {
		currTile->stopAllActions();
		currTile->removeFromParent();
		prevTile->removeFromParent();
		puzzle::Tile * t = puzzle::Tile::create(currTile->getValue() * 2);
		t->setPosition(this->caculateTilePos(targetRow, col));
		this->addChild(t, ZORDER_TILE_LAYER);
		this->tiles[targetRow][col] = t;
	});
	auto endcallback = CallFunc::create(std::bind(&GameScene::operateEndCallback, this, currTile->getValue()*2));
	currTile->runAction(Sequence::create(mtAction, func, endcallback, nullptr));
}

void GameScene::moveTiles(int targetRow, int col, puzzle::Tile* currTile)
{
	state = GameState::moving;
	this->moveCount++;
	auto mtAction = MoveTo::create(0.1f, caculateTilePos(targetRow, col));
	auto func = CallFunc::create([this, targetRow, col, currTile]()->void {
		currTile->stopAllActions();
		this->tiles[targetRow][col] = currTile;
	});
	auto endcallback = CallFunc::create(std::bind(&GameScene::operateEndCallback, this, 0));
	currTile->runAction(Sequence::create(mtAction, func, endcallback, nullptr));
}

void GameScene::addScore(int a)
{
	this->deltaScore = 0;
	score += a;
	char buff[20];
	sprintf(buff, "%d", score);
	if (bestScore < score) {
		bestScore = score;
		bestValLabel->setString(buff);
	}
	scoreValLabel->setString(buff);
	if (a > 0) {
		auto find = scoreValLabel->getParent()->getChildByTag(TAG_ADD);
		if (find) {
			find->stopAllActions();
			find->removeFromParent();
		}
		sprintf(buff, "+%d", a);
		auto addLabel = Label::createWithTTF(TTFConfig(SCORE_FONT_NAME, 24), buff);
		addLabel->setTag(TAG_ADD);
		addLabel->setColor(Color3B(0xf0, 0, 0));
		auto moveupAction = MoveBy::create(0.6, Vec2(0, 30));
		auto delSelf = RemoveSelf::create(true);
		addLabel->setPosition(Vec2(scoreValLabel->getPosition().x, scoreValLabel->getPosition().y+10));
		scoreValLabel->getParent()->addChild(addLabel, ZORDER_RESULT_LAYER);
		addLabel->runAction(Sequence::create(moveupAction, delSelf, nullptr));
	}
}

void GameScene::showResult()
{
	Size size = Director::getInstance()->getVisibleSize();
	auto sp = Sprite::create("item.png");
	auto label = Label::createWithTTF(TTFConfig(TILE_FONT_NAME, 14), "Game Over");
	if (GameState::gameover == state) {
		label->setString("Game Over");
		label->setColor(Color3B(0xff, 0x10, 0x10));
	}
	else if (GameState::win == state) {
		label->setString("Win");
		label->setColor(Color3B(0x10, 0xff, 0x10));
	}
	else {
		label->setString("Unkonw");
	}
	label->setPosition(sp->getContentSize().width/2, sp->getContentSize().height/2);
	sp->addChild(label);
	sp->setScaleX(2.4);
	sp->setScaleY(1.4);
	sp->setColor(Color3B(120, 40, 30));
	sp->setOpacity(150);
	sp->setPosition(Vec2(size.width/2, size.height/2));
	sp->setTag(TAG_RESULT_NODE);
	this->addChild(sp, ZORDER_RESULT_LAYER);
	/////////
	if (resultCallback) {
		this->scheduleOnce([this](float f)->void {
			this->resultCallback();
		}, 2, "call");
	}
}

void GameScene::operateEndCallback(int add_score)
{
	moveCount--;
	this->deltaScore += add_score;
	if (state == GameState::win)
		return;
	if (add_score == target_val) {
		state = GameState::win;
		showResult();
		return ;
	}
	//all move operate has finished, generate a tile
	if (moveCount == 0) {
		this->addScore(deltaScore);
		puzzle::Tile * t = this->genRandomTile();
		this->addChild(t, ZORDER_TILE_LAYER);
		if (!canMove()) {
			state = GameState::gameover;
			showResult();
			return;
		}
		else {
			state = GameState::running;
		}
	}
}

void GameScene::operateTiles(EMoveTo mt)
{
	if (mt == EMoveTo::down) {
		log("move down...");
		for (int col = 0; col < 4; col++) {
			int targetRow = 3;
			puzzle::Tile* prevTile = nullptr;
			for (int row = 3; row >= 0; row--) {
				if (tiles[row][col] != nullptr) {
					auto currTile = tiles[row][col];
					if (prevTile != nullptr && prevTile->getValue() == currTile->getValue()) {
						//merge
						this->mergeTiles(targetRow+1, col, prevTile, currTile);
						tiles[row][col] = nullptr;
						prevTile = nullptr;
						continue;
					}
					else if (targetRow != row) {
						this->moveTiles(targetRow, col, currTile);
						tiles[row][col] = nullptr;
					}
					prevTile = currTile;
					targetRow--;
				}
			}
		}
	}
	else if (mt == EMoveTo::up) {
		for (int col = 0; col < 4; col++) {
			int targetRow = 0;
			puzzle::Tile* prevTile = nullptr;
			for (int row = 0; row < 4; row++) {
				if (tiles[row][col] != nullptr) {
					auto currTile = tiles[row][col];
					if (prevTile != nullptr && prevTile->getValue() == currTile->getValue()) {
						//merge
						this->mergeTiles(targetRow-1, col, prevTile, currTile);
						tiles[row][col] = nullptr;
						prevTile = nullptr;
						continue;
					}
					else if (targetRow != row) {
						this->moveTiles(targetRow, col, currTile);
						tiles[row][col] = nullptr;
					}
					prevTile = currTile;
					targetRow++;
				}
			}
		}
	}
	else if (mt == EMoveTo::left) {
		for (int row = 0; row < 4; row++) {
			int targetCol = 0;
			puzzle::Tile* prevTile = nullptr;
			for (int col = 0; col < 4; col++) {
				if (tiles[row][col] != nullptr) {
					auto currTile = tiles[row][col];
					if (prevTile != nullptr && prevTile->getValue() == currTile->getValue()) {
						//merge
						this->mergeTiles(row, targetCol - 1, prevTile, currTile);
						tiles[row][col] = nullptr;
						prevTile = nullptr;
						continue;
					}
					else if (targetCol != col) {
						this->moveTiles(row, targetCol, currTile);
						tiles[row][col] = nullptr;
					}
					prevTile = currTile;
					targetCol++;
				}
			}
		}
	}
	else if (mt == EMoveTo::right){
		for (int row = 0; row < 4; row++) {
			int targetCol = 3;
			puzzle::Tile* prevTile = nullptr;
			for (int col = 3; col >= 0; col--) {
				if (tiles[row][col] != nullptr) {
					auto currTile = tiles[row][col];
					if (prevTile != nullptr && prevTile->getValue() == currTile->getValue()) {
						//merge
						this->mergeTiles(row, targetCol + 1, prevTile, currTile);
						tiles[row][col] = nullptr;
						prevTile = nullptr;
						continue;
					}
					else if (targetCol != col) {
						this->moveTiles(row, targetCol, currTile);
						tiles[row][col] = nullptr;
					}
					prevTile = currTile;
					targetCol--;
				}
			}
		}
	}
	else {
		log("error");
	}
}


bool GameScene::singleTouchBegan(Touch *touch, Event *event)
{
	Vec2 p = touch->getLocation();
	if (touchRect.containsPoint(p) && state == GameState::running) {
		beginPos = endPos = p;
	}
	else {
		beginPos = Vec2(0, 0);
		log("touchRect.is=%d,state=%d", touchRect.containsPoint(p), (int)state);
	}
	return true;
}

void GameScene::singleTouchMoved(Touch *touch, Event *event)
{
	endPos = touch->getLocation();
}

void GameScene::singleTouchEnded(Touch *touch, Event *event)
{
	if (beginPos.x == 0 && beginPos.y == 0) return;
	auto horizatal = endPos.x - beginPos.x;
	auto vertical = endPos.y - beginPos.y;
	log("singleTouchEnded : %f,%f",horizatal, vertical);
	if (abs(horizatal) > abs(vertical)) {
		if (horizatal > 0 && horizatal > min_valid_move) {
			//right
			this->operateTiles(EMoveTo::right);
		}
		else if (horizatal < 0 && min_valid_move + horizatal < 0) {
			//left
			this->operateTiles(EMoveTo::left);
		}
	}
	else {
		if (vertical > 0 && vertical > min_valid_move) {
			//up
			this->operateTiles(EMoveTo::up);
		}
		else if (vertical < 0 && min_valid_move + vertical < 0) {
			//down
			this->operateTiles(EMoveTo::down);
		}
	}
}

void GameScene::singleTouchCancelled(Touch *touch, Event *event)
{
}

void GameScene::onKeyDown(EventKeyboard::KeyCode code, Event* event)
{
}

void GameScene::onKeyUp(EventKeyboard::KeyCode code, Event* event)
{
}

void GameScene::onNewGame(cocos2d::Ref *sender)
{
	auto node = this->getChildByTag(TAG_RESULT_NODE);
	if (node) {
		node->removeFromParent();
	}
	this->clearTiles();
	this->resetGame();
	auto t1 = genRandomTile();
	auto t2 = genRandomTile();
	this->addChild(t1, ZORDER_TILE_LAYER);
	this->addChild(t2, ZORDER_TILE_LAYER);
	state = GameState::running;
}

void GameScene::onAI(cocos2d::Ref *sender)
{
	this->schedule(std::bind(&GameScene::moveAI, this, _1), 0.25, -1, 0, "AI");
}

void GameScene::onTest(cocos2d::Ref* sender)
{
	static int count = 0;
	static int wincount = 0;
	onAI(nullptr);
	resultCallback = [&]()->void {
		if (this->state == GameState::win) {
			wincount++;
		}
		count++;
		log("++++++++++count=%d, wincount=%d", count, wincount);
		this->onNewGame(nullptr);
		this->unschedule("AI");
		onAI(nullptr);
	};
}

void GameScene::moveAI(float f)
{
	if (this->state == GameState::moving) return;
	if (this->state != GameState::running) {
		log("this->state=%d", this->state);
		this->unschedule("AI");
		return;
	}
	SimulateGame sim(this->tiles, this->score);
	int val = sim.getBestDirection();
	CCASSERT(val != -1, "error : val == -1.....");
	operateTiles((EMoveTo)val);
}

void GameScene::resetGame()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			tiles[i][j] = nullptr;
		}
	}
	addScore(-score);
}

void GameScene::clearTiles()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (tiles[i][j] != nullptr){
				tiles[i][j]->removeFromParent();
			}
		}
	}
}

int GameScene::getEmptySoltNum()
{
	int cnt = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (tiles[i][j] == nullptr){
				cnt++;
			}
		}
	}
	return cnt;
}

bool GameScene::canMove()
{
	if (getEmptySoltNum() > 0) {
		return true;
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i > 0 && tiles[i-1][j]->getValue() == tiles[i][j]->getValue()) {
				return true;
			}
			if (i+1 < 4 && tiles[i + 1][j]->getValue() == tiles[i][j]->getValue()) {
				return true;
			}
			if (j > 0 && tiles[i][j - 1]->getValue() == tiles[i][j]->getValue()) {
				return true;
			}
			if (j+1 < 4 && tiles[i][j + 1]->getValue() == tiles[i][j]->getValue()) {
				return true;
			}
		}
	}
	return false;
}

bool GameScene::hasWin()
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (tiles[i][j] != NULL && tiles[i][j]->getValue() == target_val)
				return true;
		}
	}
	return false;
}

puzzle::Tile *GameScene::genRandomTile()
{
	int randpos = cocos2d::random<int>(1, this->getEmptySoltNum());
	int randval = cocos2d::random<int>(1, 10) > 5 ? 4 : 2;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (tiles[i][j] == nullptr){
				if (randpos == 1) {
					auto t = puzzle::Tile::create(randval);
					t->setPosition(caculateTilePos(i, j));
					tiles[i][j] = t;
					return t;
				}
				else {
					randpos--;
				}
			}
		}
	}
	return nullptr;
}

Vec2 GameScene::caculateTilePos(int row, int col)
{
	auto vsize = CCDirector::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	return Vec2(vsize.width*(0.185 + col*0.210) + origin.x, vsize.height*(0.52 - row*0.14) + origin.y);
}
