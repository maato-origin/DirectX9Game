#ifndef _SPACEWAR_H
#define _SPACEWAR_H
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"

//このクラスはゲームエンジンのコア
class SpaceWar : public Game
{
private:
	//ゲームアイテム
	TextureManager nebulaTexture;		//星雲のテクスチャ
	TextureManager planetTexture;		//惑星のテクスチャ
	TextureManager shipTexture;			//宇宙船のテクスチャ
	Image planet;						//惑星の画像
	Image nebula;						//星雲の画像
	Image ship;							//宇宙船の画像

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
	void releaseAll();
	void resetAll();
};

#endif // !_SPACEWAR_H
