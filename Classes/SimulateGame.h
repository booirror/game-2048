#ifndef _Simulate_Game_h
#define _Simulate_Game_h
#include "Tile.h"
#include "common.h"
#define DEFAULT_NUM 3000
struct GameData{
	int score = 0;
	int tiles[4][4];
	GameData() :score(0) {}
	GameData(const GameData& gd):score(0) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				tiles[i][j] = gd.tiles[i][j];
			}
		}
	}
};

class SimulateGame {
public:
	SimulateGame(puzzle::Tile* data[4][4], int score);
	~SimulateGame() {}
	void setSimulateTimes(int n) { num = n; }
	int getBestDirection();
private:
	void randVal(GameData &data);
	bool canMove(GameData &d, EMoveTo mt);
	void move(GameData &data ,EMoveTo mt);
	int simMoveOnce(EMoveTo to);
	int getScore();
	GameState state = GameState::running;
	GameData gameData;
	int prevScore = 0;
	int num = DEFAULT_NUM;
};

#endif