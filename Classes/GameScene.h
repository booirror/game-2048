#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Tile.h"
#include "common.h"
USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
	GameScene(){}
	~GameScene(){}

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

private:
	void moveTiles(int targetRow, int col, puzzle::Tile* currTile);
	void mergeTiles(int targetRow, int col, puzzle::Tile* prevTile, puzzle::Tile* currTile);
	void operateEndCallback(int add_score);
	void resetGame();
	void clearTiles();
	void addScore(int s);
	bool canMove();
	bool hasWin();
	int getEmptySoltNum();
	void showResult();
	void moveAI(float f);

	puzzle::Tile *genRandomTile();
	cocos2d::Vec2 caculateTilePos(int row, int col);
	void operateTiles(EMoveTo mt);
	void onNewGame(cocos2d::Ref *sender);
	void onAI(cocos2d::Ref *sender);
	void onTest(cocos2d::Ref* sender);

	bool singleTouchBegan(Touch *touch, Event *event);
	void singleTouchMoved(Touch *touch, Event *event);
	void singleTouchEnded(Touch *touch, Event *event);
	void singleTouchCancelled(Touch *touch, Event *event);

	void onKeyDown(EventKeyboard::KeyCode code, Event* event);
	void onKeyUp(EventKeyboard::KeyCode code, Event* event);

	Rect touchRect; //valid touch rect
	Vec2 beginPos;
	Vec2 endPos;
	//[0][0],[0][1],[0][2],[0][3]
	//[1][0],[1][1],[1][2],[1][3]
	//[2][0],[2][1],[2][2],[2][3]
	//[3][0],[3][1],[3][2],[3][3]
	puzzle::Tile *tiles[4][4];

	int score = 0;
	int bestScore = 0;
	int deltaScore = 0;
	int moveCount = 0;
	Label * scoreValLabel;
	Label * bestValLabel;
	std::function<void()> resultCallback = nullptr;
	GameState state = GameState::prepare;
};

#endif // __GAME_SCENE_H__
