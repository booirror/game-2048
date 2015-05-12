#include "SimulateGame.h"
#include "cocos2d.h"
SimulateGame::SimulateGame(puzzle::Tile *data[4][4], int score)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (data[i][j]) {
				//cocos2d::log("%d", data[i][j]->getValue());
			}
			gameData.tiles[i][j] = data[i][j] == nullptr ? 0 : data[i][j]->getValue();
		}
	}
	/*for (int i = 0; i < 4; i++) {
		cocos2d::log("%d\t%d\t%d\t%d", gameData.tiles[i][0], gameData.tiles[i][1], gameData.tiles[i][2], gameData.tiles[i][3]);
	}*/
	this->prevScore = score;
}

static bool isGameOver(int tiles[4][4])
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (tiles[i][j] == 0)
				return false;
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i > 0 && tiles[i - 1][j]== tiles[i][j]) {
				return false;
			}
			if (i + 1 < 4 && tiles[i + 1][j] == tiles[i][j]) {
				return false;
			}
			if (j > 0 && tiles[i][j - 1] == tiles[i][j]) {
				return false;
			}
			if (j + 1 < 4 && tiles[i][j + 1] == tiles[i][j]) {
				return false;
			}
		}
	}
	return true;
}

static bool canWin(int tiles[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (tiles[i][j] == 2048)
				return true;
		}
	}
	return false;
}

bool SimulateGame::canMove(GameData& d, EMoveTo mt)
{
	GameData temp(d);
	move(temp, mt);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (d.tiles[i][j] != temp.tiles[i][j])
				return true;
		}
	}
	return false;
}

void SimulateGame::randVal(GameData &data)
{
	int randvalue = (rand() % 10) > 8 ? 4 : 2;
	int randx = rand()%4;
	int randy = rand()%4;
	if (data.tiles[randx][randy] != 0) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (data.tiles[i][j] == 0)
					data.tiles[i][j] = randvalue;
			}
		}
	}
	else {
		data.tiles[randx][randy] = randvalue;
	}
}

int SimulateGame::simMoveOnce(EMoveTo mt)
{
	GameData data(this->gameData);
	if (!canMove(data, mt)) return 0;
	int cnt = 0;
	while (!isGameOver(data.tiles)) {
		move(data, mt);
		if (canWin(data.tiles)) {
			data.score += 10000;
			break;
		}
		mt = (EMoveTo)(rand() % 4);
		this->randVal(data);
	}
	return data.score == 0 ? 1 : data.score;
}

void SimulateGame::move(GameData &data ,EMoveTo mt)
{
	if (mt == EMoveTo::down) {
		for (int col = 0; col < 4; col++) {
			int targetRow = 3;
			int prevTile = 0;
			for (int row = 3; row >= 0; row--) {
				if (data.tiles[row][col] != 0) {
					auto currTile = data.tiles[row][col];
					if (prevTile != 0 && prevTile == currTile) {
						//merge
						//this->mergeTiles(targetRow + 1, col, prevTile, currTile);
						data.tiles[targetRow + 1][col] = currTile * 2;
						data.tiles[row][col] = 0;
						prevTile = 0;
						data.score += currTile * 2;
						continue;
					}
					else if (targetRow != row) {
						data.tiles[targetRow][col] = currTile;
						data.tiles[row][col] = 0;
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
			int prevTile = 0;
			for (int row = 0; row < 4; row++) {
				if (data.tiles[row][col] != 0) {
					auto currTile = data.tiles[row][col];
					if (prevTile != 0 && prevTile == currTile) {
						//merge
						//this->mergeTiles(targetRow - 1, col, prevTile, currTile);
						data.tiles[targetRow - 1][col] = currTile * 2;
						data.tiles[row][col] = 0;
						prevTile = 0;
						data.score += currTile * 2;
						continue;
					}
					else if (targetRow != row) {
						data.tiles[targetRow][col] = currTile;
						data.tiles[row][col] = 0;
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
			int prevTile = 0;
			for (int col = 0; col < 4; col++) {
				if (data.tiles[row][col] != 0) {
					auto currTile = data.tiles[row][col];
					if (prevTile != 0 && prevTile == currTile) {
						//merge
						//this->mergeTiles(row, targetCol - 1, prevTile, currTile);
						data.tiles[row][targetCol - 1] = currTile * 2;
						data.tiles[row][col] = 0;
						prevTile = 0;
						data.score += currTile * 2;
						continue;
					}
					else if (targetCol != col) {
						//this->moveTiles(row, targetCol, currTile);
						data.tiles[row][targetCol] = currTile;
						data.tiles[row][col] = 0;
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
			int prevTile = 0;
			for (int col = 3; col >= 0; col--) {
				if (data.tiles[row][col] != 0) {
					auto currTile = data.tiles[row][col];
					if (prevTile != 0 && prevTile == currTile) {
						//merge
						//this->mergeTiles(row, targetCol + 1, prevTile, currTile);
						data.tiles[row][targetCol + 1] = currTile * 2;
						data.tiles[row][col] = 0;
						prevTile = 0;
						data.score += currTile * 2;
						continue;
					}
					else if (targetCol != col) {
						//this->moveTiles(row, targetCol, currTile);
						data.tiles[row][targetCol] = currTile;
						data.tiles[row][col] = 0;
					}
					prevTile = currTile;
					targetCol--;
				}
			}
		}
	}
	else {
		//log("error");
	}
}

int SimulateGame::getBestDirection()
{
	int sum[4] = { 0 };
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < num; j++) {
			sum[i] += simMoveOnce((EMoveTo)i);
		}
	}

	int max = 0;
	for (int i = 0; i < 4; i++) {
		if (sum[i] > sum[max]) {
			max = i;
		}
	}
	if (sum[max] == 0) return -1;
	return max;
}

