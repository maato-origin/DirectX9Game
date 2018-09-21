#ifndef _SPACEWAR_H
#define _SPACEWAR_H
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "planet.h"
#include "ship.h"

namespace spacewarNS
{
	const char FONT[] = "Arial Bold";	//フォント
	const int FONT_BIG_SIZE = 256;		//フォントの高さ
	const int FONT_SCORE_SIZE = 48;
	const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
	const COLOR_ARGB SHIP1_COLOR = graphicsNS::BLUE;
	const int SCORE_Y = 10;
	const int SCORE1_X = 60;
	const int SHIP1_HEALTHBAR_X = 40;
	const int HEALTHBAR_START_FRAME = 22;
	const int HEALTHBAR_END_FRAME = 22;
	const int HEALTHBAR_HEIGHT = 32;
	const int HEALTHBAR_WIDTH = 64;
	const int COUNT_DOWN_X = GAME_WIDTH / 2 - FONT_BIG_SIZE / 4;
	const int COUNT_DOWN_Y = GAME_HEIGHT / 2 - FONT_BIG_SIZE / 2;
	const int COUNT_DOWN = 2;			//カウントダウン
	const int BUF_SIZE = 20;
	const int ROUND_TIME = 5;			//新しいラウンド開始までの時間
	const int HEALTHBAR_TEXTURE_COLS = 4;	//体力バーに4列分テクスチャを使用
}

//このクラスはゲームエンジンのコア
class SpaceWar : public Game
{
private:
	//ゲームアイテム
	TextureManager menuTexture, spaceTexture, textures1;
	Ship ship1;					//宇宙船のクラス
	Image space;						//星空の画像
	Image planet;
	Image moons[4];
	Image menu;							//メニューの画像
	bool menuOn;
	bool countDownOn;					//カウントダウンが表示されるときtrue
	float countDownTimer;
	TextDX fontBig;						//ゲームバナー用DirectXフォント
	TextDX fontScore;
	char buffer[spacewarNS::BUF_SIZE];
	bool ship1Scored;					//ラウンドの宇宙船スコアを記録したらtrue
	bool roundOver;						//ラウンドを越えたらtrue
	float roundTimer;					//新しいラウンド開始までの時間
	int ship1Score;						//スコア

public:
	//コンストラクタ
	SpaceWar();
	
	//デストラクタ
	virtual ~SpaceWar();

	//ゲームを初期化
	void initialize(HWND hwnd);
	void update();		//Gameからの純粋仮想関数をオーバーライド
	void ai();
	void collisions();
	void render();
	void roundStart();	//新たなゲームラウンドを開始
	void releaseAll();
	void resetAll();
};

#endif // !_SPACEWAR_H
