#ifndef _2048_common_h
#define _2048_common_h

#define GAME_FONT_NAME "fonts/tahoma.ttf"
#define TILE_FONT_NAME "fonts/tahoma.ttf"
#define SCORE_FONT_NAME "fonts/tahoma.ttf"

enum class EMoveTo{
	left,
	right,
	up,
	down
};

enum class GameState{
	prepare,
	running,
	moving,
	gameover,
	win
};


#endif